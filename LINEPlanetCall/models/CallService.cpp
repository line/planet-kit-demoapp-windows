// Copyright 2024 LINE Plus Corporation
//
// LINE Plus Corporation licenses this file to you under the Apache License,
// version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at:
//
//   https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#include "pch.h"

#include "CallService.h"

#if __has_include("CallService.g.cpp")
#include "CallService.g.cpp"
#endif

#include <strsafe.h>

namespace winrt::LINEPlanetCall::implementation {
    CallService::CallService(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;

        m_callTitleTimer.Interval(std::chrono::milliseconds{ Constants::connectedChecker });
        m_callTitleTimerToken = m_callTitleTimer.Tick({ this, &CallService::onTitleTimer });
    }

    CallService::~CallService() {
        m_callTitleTimer.Tick(m_callTitleTimerToken);
    }

    event_token CallService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CallService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring CallService::peerId() {
        return m_peerId;
    }

    PlanetKit::PlanetKitCallPtr CallService::planetKitCall() {
        return m_planetKitCall;
    }

    void CallService::peerId(hstring const& peerId) {
        if (m_peerId != peerId) {
            m_peerId = peerId;
            dispatchPropertyChanged(L"peerId");
        }
    }

    ConnectState CallService::callState() {
        return m_callState;
    }

    void CallService::callState(ConnectState callState) {
        if (m_callState != callState) {
            m_callState = callState;
            dispatchPropertyChanged(L"callState");
        }
    }

    bool CallService::peerMute() {
        return m_peerMute;
    }

    bool CallService::myMute() {
        return m_myMute;
    }

    PlanetKitMediaType CallService::mediaType() {
        return m_mediaType;
    }

    bool CallService::myVideoEnabled() {
        return m_myVideoEnabled;
    }

    bool CallService::peerVideoEnabled() {
        return m_peerVideoEnabled;
    }

    int32_t CallService::peerVolume() {
        return m_peerVolume;
    }

    int32_t CallService::disconnectReason() {
        return m_disconnectReason;
    }

    hstring CallService::alertMessage() {
        return m_alertMessage;
    }

    Windows::Foundation::IAsyncOperation<ConnectResult> CallService::makeCall(hstring userId, hstring accessToken, bool videoCall, hstring audioInputDeviceUid, hstring audioOutputDeviceUid) {
        ConnectResult result{ false, ErrorType::none, AppFailedReason::none, (int32_t)PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_NONE };
        if (userId.empty()) {
            result.errorType = ErrorType::appError;
            result.appFailedReason = AppFailedReason::emptyId;

            co_return result;
        }
        else if (m_peerId.empty()) {
            result.errorType = ErrorType::appError;
            result.appFailedReason = AppFailedReason::emptyPeerId;

            co_return result;
        }
        
        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background(); // Return control; resume on thread pool.

        AppServerController appServerController;
        auto gwAccessToken = appServerController.accessToken(accessToken).get();

        co_await ui_thread; // await ui thread

        if (gwAccessToken.empty()) {
            result.errorType = ErrorType::appError;
            result.appFailedReason = AppFailedReason::expiredAccessToken;

            co_return result;
        }

        // Create my id and peer id.
        LINEPlanetCall::ServerInfoData serverInfoData;
        auto planetMyId = PlanetKit::UserId::Create(userId.c_str(), serverInfoData.serviceId().c_str());
        auto planetPeerId = PlanetKit::UserId::Create(m_peerId.c_str(), serverInfoData.serviceId().c_str());

        // Create `MakeCallParam`.
        auto makeCallParam = PlanetKit::MakeCallParam::CreateWithAccessToken(planetMyId, planetPeerId, gwAccessToken.c_str());
        makeCallParam->SetIsVideoCall(videoCall);
        
        auto inputDevice = audioDevice(PlanetKitAudioDeviceType::input, audioInputDeviceUid);
        auto outputDevice = audioDevice(PlanetKitAudioDeviceType::output, audioOutputDeviceUid);

        if (*inputDevice != nullptr) {
            m_input = inputDevice;
            makeCallParam->SetAudioInputDevice(inputDevice);
        }
        
        if (*outputDevice != nullptr) {
            m_output = outputDevice;
            makeCallParam->SetAudioOutputDevice(outputDevice);
        }

        makeCallParam->SetCallEvent(&m_callEventListener);

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto makeCallResult = planetKitManager->MakeCall(makeCallParam, &m_planetKitCall);

        
        result.connect = makeCallResult.bSuccess;

        if (makeCallResult.bSuccess == true) {
            // update call state
            callState(ConnectState::waitAnswer);

            m_parentWindow.Title(m_peerId);

            if (videoCall == true) {
                m_mediaType = PlanetKitMediaType::audioVideo;
            }
            else {
                m_mediaType = PlanetKitMediaType::audio;
            }

            dispatchPropertyChanged(L"mediaType");
        }
        else {
            result.errorType = ErrorType::planetKitError;
            result.planetKitFailedReason = (int32_t)makeCallResult.reason;
        }

        co_return result;
    }

    bool CallService::verifyCall(hstring const& userId, hstring const& peerId, hstring const& ccParam, PlanetKitMediaType mediaType, hstring audioInputDeviceUid, hstring audioOutputDeviceUid) {
        LINEPlanetCall::ServerInfoData serverInfoData;
        auto planetMyId = PlanetKit::UserId::Create(userId.c_str(), serverInfoData.serviceId().c_str());

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();

        int length = WideCharToMultiByte(CP_UTF8, 0, ccParam.c_str(), -1, nullptr, 0, nullptr, nullptr);
        char* utf8CCParam = new char[length];
        WideCharToMultiByte(CP_UTF8, 0, ccParam.c_str(), -1, utf8CCParam, length, nullptr, nullptr);

        auto planetKitCCParam = planetKitManager->CreateCCParam(utf8CCParam);
        delete[] utf8CCParam;

        auto verifyCallParam = PlanetKit::VerifyCallParam::Create(planetMyId, planetKitCCParam);
        verifyCallParam->SetCallEvent(&m_callEventListener);

        auto inputDevice = audioDevice(PlanetKitAudioDeviceType::input, audioInputDeviceUid);
        auto outputDevice = audioDevice(PlanetKitAudioDeviceType::output, audioOutputDeviceUid);

        if (*inputDevice != nullptr) {
            m_input = inputDevice;
            verifyCallParam->SetAudioInputDevice(inputDevice);
        }

        if (*outputDevice != nullptr) {
            m_output = outputDevice;
            verifyCallParam->SetAudioOutputDevice(outputDevice);
        }

        auto startResult = planetKitManager->VerifyCall(verifyCallParam, &m_planetKitCall);

        if (startResult.bSuccess == true) {
            m_peerId = peerId;
            m_mediaType = mediaType;

            callState(ConnectState::verified);
        }

        return startResult.bSuccess;
    }

    void CallService::endCall() {
        m_planetKitCall->EndCall();
    }

    void CallService::acceptCall() {
        m_planetKitCall->AcceptCall(false);
    }

    void CallService::muteMyAudio() {
        bool mute = !m_myMute;

        if (m_callState != ConnectState::connected) {
            m_planetKitCall->MuteMyAudio(mute);
            m_myMute = mute;
            dispatchPropertyChanged(L"myMute");
        }
        else {
            struct MuteResult {
                bool mute;
                CallService* callService;
            }*muteResult = new MuteResult{ mute, this };

            m_planetKitCall->MuteMyAudio(mute, muteResult, [](void* pUserData, bool bSuccess) {
                MuteResult* muteResult = (MuteResult*)pUserData;

                if (bSuccess) {
                    muteResult->callService->m_myMute = muteResult->mute;
                    muteResult->callService->dispatchPropertyChanged(L"myMute");
                }

                delete muteResult;
            });
        }
    }

    void CallService::pauseMyVideo() {
        if (m_callState != ConnectState::connected) {
            if (m_myVideoEnabled == true) {
                if (*m_planetKitCall) {
                    m_planetKitCall->PauseMyVideo();
                }
                
                m_myVideoEnabled = false;
                dispatchPropertyChanged(L"myVideoEnabled");
            }
            else {
                // do nothing
            }
        }
        else {
            m_planetKitCall->PauseMyVideo(this, [](void* pUserData, bool bSuccess) {
                if (bSuccess == true) {
                    CallService* callService = (CallService*)pUserData;

                    callService->m_myVideoEnabled = false;
                    callService->dispatchPropertyChanged(L"myVideoEnabled");
                }
            });
        }
    }

    void CallService::resumeMyVideo() {
        if (m_callState != ConnectState::connected) {
            if (m_myVideoEnabled == false) {
                m_planetKitCall->ResumeMyVideo();
                m_myVideoEnabled = true;
                dispatchPropertyChanged(L"myVideoEnabled");
            }
            else {
                // do nothing
            }
        }
        else {
            m_planetKitCall->ResumeMyVideo(this, [](void* pUserData, bool bSuccess) {
                if (bSuccess == true) {
                    CallService* callService = (CallService*)pUserData;

                    callService->m_myVideoEnabled = true;
                    callService->dispatchPropertyChanged(L"myVideoEnabled");
                }
            });
        }
    }

    void CallService::audioInputDeviceChanged(hstring const& uid) {
        if (uid.empty()) {
            return;
        }

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto planetKitAudioManager = planetKitManager->GetAudioManager();

        PlanetKit::AudioDeviceInfoArray audioInfos;
        planetKitAudioManager->GetAudioInputDeviceList(&audioInfos);

        for (int32_t idx = 0; idx < audioInfos.Size(); ++idx) {
            if (audioInfos[idx]->GetID() == uid.c_str()) {
                auto inputDevice = planetKitAudioManager->CreateAudioDevice(audioInfos[idx]);
                if (*inputDevice) {

                    if (*m_planetKitCall) {
                        m_input = inputDevice;
                        m_planetKitCall->SetAudioInputDevice(inputDevice);
                    }
                    
                    Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
                    std::wstring message = resourceLoader.GetString(L"lp_demoapp_common_changemic/text").c_str();
                    message.replace(message.find(Constants::replaceMicString), Constants::replaceMicString.size(), audioInfos[idx]->GetName());
                    m_alertMessage = message.c_str();

                    dispatchPropertyChanged(L"alertMessage");
                }

                break;
            }
        }
    }

    void CallService::audioOutputDeviceChanged(hstring const& uid) {
        if (uid.empty()) {
            return;
        }

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto planetKitAudioManager = planetKitManager->GetAudioManager();

        PlanetKit::AudioDeviceInfoArray audioInfos;
        planetKitAudioManager->GetAudioOutputDeviceList(&audioInfos);

        for (int32_t idx = 0; idx < audioInfos.Size(); ++idx) {
            if (audioInfos[idx]->GetID() == uid.c_str()) {
                auto outputDevice = planetKitAudioManager->CreateAudioDevice(audioInfos[idx]);
                if (*outputDevice) {
                    m_output = outputDevice;
                    m_planetKitCall->SetAudioOutputDevice(outputDevice);
                }

                break;
            }
        }
    }

    void CallService::onConnected() {
        // update call state
        callState(ConnectState::connected);

        m_callConnectedTime = 0;
        m_callTitleTimer.Start();

        // initialize first status of peer's video
        onPeerVideoEnabled(true);
    }

    void CallService::onDisconnected(int32_t reason) {
        clear();
        m_disconnectReason = (int32_t)reason;
        callState(ConnectState::disconnected);
    }

    void CallService::onPeerMute(bool mute) {
        m_peerMute = mute;
        dispatchPropertyChanged(L"peerMute");
    }

    void CallService::onPeerVideoEnabled(bool enabled) {
        m_peerVideoEnabled = enabled;
        dispatchPropertyChanged(L"peerVideoEnabled");
    }

    void CallService::onPeerVolumeChanged(int32_t volume) {
        m_peerVolume = volume;
        dispatchPropertyChanged(L"peerVolume");
    }

    void CallService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void CallService::onTitleTimer(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_callConnectedTime++;
        std::chrono::hh_mm_ss<std::chrono::seconds> tod{ std::chrono::seconds(m_callConnectedTime) };

        hstring title;
        title = m_peerId + L" (" + to_hstring(tod.minutes().count()) + L":" + to_hstring(tod.seconds().count()) + L")";
        m_parentWindow.Title(title);
    }

    void CallService::clear() {
        m_callTitleTimer.Stop();

        m_parentWindow.Title(Constants::appTitle);

        m_input = nullptr;
        m_output = nullptr;
        
        m_planetKitCall = nullptr;
        m_peerId.clear();

        m_peerMute = false;
        m_myMute = false;
        m_myVideoEnabled = true;
        m_peerVideoEnabled = false;
    }

    PlanetKit::AudioDevicePtr CallService::audioDevice(PlanetKitAudioDeviceType type, hstring const& uid) {
        PlanetKit::AudioDevicePtr audioDevice;

        if (uid.empty()) {
            return audioDevice;
        }
        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto audioDeviceManager = planetKitManager->GetAudioManager();
        PlanetKit::AudioDeviceInfoArray audioInfos;

        if (type == PlanetKitAudioDeviceType::input) {
            audioDeviceManager->GetAudioInputDeviceList(&audioInfos);
        }
        else {
            audioDeviceManager->GetAudioOutputDeviceList(&audioInfos);
        }

        for (int32_t idx = 0; idx < audioInfos.Size(); ++idx) {
            if (audioInfos[idx]->GetID() == uid.c_str()) {
                // found matching device
                audioDevice = audioDeviceManager->CreateAudioDevice(audioInfos[idx]);

                break;
            }
        }

        return audioDevice;
    }
}
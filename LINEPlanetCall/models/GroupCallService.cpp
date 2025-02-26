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

#include "GroupCallService.h"

#if __has_include("GroupCallService.g.cpp")
#include "GroupCallService.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    GroupCallService::GroupCallService(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;

        m_groupCallTitleTimer.Interval(std::chrono::milliseconds{ Constants::connectedChecker });
        m_groupCallTitleTimerToken = m_groupCallTitleTimer.Tick({ this, &GroupCallService::onTitleTimer });
    }

    GroupCallService::~GroupCallService() {
        m_groupCallTitleTimer.Tick(m_groupCallTitleTimerToken);
    }

    void GroupCallService::registerParticipantUpdatedEvent(ParticipantUpdatedEventListener* eventListener) {
        m_participantUpdatedEventListener = eventListener;
    }

    void GroupCallService::unregisterParticipantUpdatedEvent() {
        m_participantUpdatedEventListener = nullptr;
    }

    void GroupCallService::onPeerListUpdated(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        if (m_participantUpdatedEventListener) {
            m_participantUpdatedEventListener->participantUpdated(pParam);
        }
    }

    event_token GroupCallService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring GroupCallService::roomId() {
        return m_roomId;
    }

    void GroupCallService::roomId(hstring const& roomId) {
        m_roomId = roomId;

        dispatchPropertyChanged(L"roomId");
    }

    ConnectState GroupCallService::groupCallState() {
        return m_groupCallState;
    }

    bool GroupCallService::myMute() {
        return m_myMute;
    }

    bool GroupCallService::myVideoEnabled() {
        return m_myVideoEnabled;
    }

    int32_t GroupCallService::disconnectReason() {
        return m_disconnectReason;
    }

    hstring GroupCallService::alertMessage() {
        return m_alertMessage;
    }

    void GroupCallService::clear() {
        m_myMute = false;
        m_myVideoEnabled = true;

        m_input = nullptr;
        m_output = nullptr;

        m_disconnectReason = 0;
        m_roomId.clear();

        m_planetKitConference = nullptr;
    }

    Windows::Foundation::IAsyncOperation<ConnectResult> GroupCallService::joinConference(hstring userId, hstring userName, hstring accessToken, hstring audioInputDeviceUid, hstring audioOutputDeviceUid) {
        ConnectResult result = { false, ErrorType::none, AppFailedReason::none, (int32_t)PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_NONE };
        if (userId.empty()) {
            result.errorType = ErrorType::appError;
            result.appFailedReason = AppFailedReason::emptyId;

            clear();

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

            clear();

            co_return result;
        }

        // Create my id
        LINEPlanetCall::ServerInfoData serverInfoData;
        PlanetKit::WStringOptional counrtyCode(L"KR");
        auto planetKitMyId = PlanetKit::UserId::Create(userId.c_str(), serverInfoData.serviceId().c_str(), counrtyCode);

        auto conferenceParam = PlanetKit::ConferenceParam::CreateWithAccessToken(planetKitMyId, m_roomId.c_str(), serverInfoData.serviceId().c_str(), gwAccessToken.c_str());
        conferenceParam->SetIsVideoCall(true);
        conferenceParam->SetDisplayName(PlanetKit::WString(userName.c_str()));

        auto inputDevice = audioDevice(PlanetKitAudioDeviceType::input, audioInputDeviceUid);
        auto outputDevice = audioDevice(PlanetKitAudioDeviceType::output, audioOutputDeviceUid);

        if (*inputDevice != nullptr) {
            conferenceParam->SetAudioInputDevice(inputDevice);
            m_input = inputDevice;
        }

        if (*outputDevice != nullptr) {
            conferenceParam->SetAudioOutputDevice(outputDevice);
            m_output = outputDevice;
        }

        conferenceParam->SetConferenceEvent(&m_groupCallEventListener);

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto joinConferenceResult = planetKitManager->JoinConference(conferenceParam, m_planetKitConference);

        result.connect = joinConferenceResult.bSuccess;

        if (joinConferenceResult.bSuccess == true) {
            if (m_myMute == true) {
                m_planetKitConference->MuteMyAudio(true);
            }

            if (m_myVideoEnabled == false) {
                m_planetKitConference->PauseMyVideo();
            }
        }
        else {
            result.errorType = ErrorType::planetKitError;
            result.planetKitFailedReason = (int32_t)joinConferenceResult.reason;

            clear();
        }

        co_return result;
    }

    void GroupCallService::leaveConference() {
        m_planetKitConference->LeaveConference();
    }

    void GroupCallService::muteMyAudio() {
        bool mute = !m_myMute;

        if (m_groupCallState != ConnectState::connected) {
            m_myMute = mute;
            dispatchPropertyChanged(L"myMute");
        }
        else {
            struct MuteResult {
                bool mute;
                GroupCallService* groupCallService;
            }*muteResult = new MuteResult{ mute, this };

            m_planetKitConference->MuteMyAudio(mute, muteResult, [](void* pUserData, bool bSuccess) {
                MuteResult* muteResult = (MuteResult*)pUserData;

                if (bSuccess) {
                    muteResult->groupCallService->m_myMute = muteResult->mute;
                    muteResult->groupCallService->dispatchPropertyChanged(L"myMute");
                }

                delete muteResult;
            });
        }
    }

    void GroupCallService::pauseMyVideo() {
        if (m_groupCallState != ConnectState::connected) {
            if (m_myVideoEnabled == true) {
                m_myVideoEnabled = false;
                dispatchPropertyChanged(L"myVideoEnabled");
            }
        }
        else {
            m_planetKitConference->PauseMyVideo(this, [](void* pUserData, bool bSuccess) {
                if (bSuccess == true) {
                    GroupCallService* groupCallService = (GroupCallService*)pUserData;

                    groupCallService->m_myVideoEnabled = false;
                    groupCallService->dispatchPropertyChanged(L"myVideoEnabled");
                }
            });
        }
    }

    void GroupCallService::resumeMyVideo() {
        if (m_groupCallState != ConnectState::connected) {
            if (m_myVideoEnabled == false) {
                m_myVideoEnabled = true;
                dispatchPropertyChanged(L"myVideoEnabled");
            }
        }
        else {
            m_planetKitConference->ResumeMyVideo(this, [](void* pUserData, bool bSuccess) {
                if (bSuccess == true) {
                    GroupCallService* groupCallService = (GroupCallService*)pUserData;

                    groupCallService->m_myVideoEnabled = true;
                    groupCallService->dispatchPropertyChanged(L"myVideoEnabled");
                }
            });
        }
    }


    void GroupCallService::audioInputDeviceChanged(hstring const& uid) {
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
                    if (*m_planetKitConference) {
                        m_input = inputDevice;
                        m_planetKitConference->SetAudioInputDevice(inputDevice);
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

    void GroupCallService::audioOutputDeviceChanged(hstring const& uid) {
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
                    m_planetKitConference->SetAudioOutputDevice(outputDevice);
                }

                break;
            }
        }
    }

    void GroupCallService::onConnected() {
        m_groupCallState = ConnectState::connected;

        m_callConnectedTime = 0;
        m_groupCallTitleTimer.Start();

        dispatchPropertyChanged(L"groupCallState");
    }

    void GroupCallService::onDisconnected(int32_t reason) {
        clear();

        m_groupCallState = ConnectState::disconnected;
        m_disconnectReason = reason;

        m_groupCallTitleTimer.Stop();
        m_parentWindow.Title(Constants::appTitle);

        dispatchPropertyChanged(L"groupCallState");
    }

    PlanetKit::AudioDevicePtr GroupCallService::audioDevice(PlanetKitAudioDeviceType type, hstring const& uid) {
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
    
    void GroupCallService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void GroupCallService::onTitleTimer(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_callConnectedTime++;
        std::chrono::hh_mm_ss<std::chrono::seconds> tod{ std::chrono::seconds(m_callConnectedTime) };

        hstring title;
        title = m_roomId + L" (" + to_hstring(tod.minutes().count()) + L":" + to_hstring(tod.seconds().count()) + L")";
        m_parentWindow.Title(title);
    }
}
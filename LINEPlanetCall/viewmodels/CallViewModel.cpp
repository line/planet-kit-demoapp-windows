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

#include "CallViewModel.h"

#if __has_include("CallViewModel.g.cpp")
#include "CallViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    CallViewModel::CallViewModel(LINEPlanetCall::SettingService settigService, LINEPlanetCall::CallService callService) {
        m_settigService = settigService;
        m_callService = callService;

        // register event
        m_settingServiceToken = m_settigService.PropertyChanged({ this, &CallViewModel::serviceProperyChanged });
        m_callServiceToken = m_callService.PropertyChanged({ this, &CallViewModel::serviceProperyChanged });

        m_alertMessageShowTimer.Interval(std::chrono::milliseconds{ Constants::alertTimer });
        m_alertMessageShowTimerToken = m_alertMessageShowTimer.Tick({ this, &CallViewModel::onTick });
    }

    CallViewModel::~CallViewModel() {
        // deregister event
        m_alertMessageShowTimer.Tick(m_alertMessageShowTimerToken);
        m_settigService.PropertyChanged(m_settingServiceToken);
        m_callService.PropertyChanged(m_callServiceToken);
    }

    event_token CallViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CallViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring CallViewModel::peerId() {
        return m_callService.peerId();
    }

    void CallViewModel::peerId(hstring const& peerId) {
        if (m_callService.peerId() != peerId) {
            m_callService.peerId(peerId);
            dispatchPropertyChanged(L"peerId");
        }
    }

    ConnectState CallViewModel::callState() {
        return m_callService.callState();
    }

    Microsoft::UI::Xaml::Visibility CallViewModel::myMute() {
        return m_callService.myMute() == true ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }

    Microsoft::UI::Xaml::Visibility CallViewModel::myVideoPaused() {
        return m_callService.myVideoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    bool CallViewModel::myVideoEnabled() {
        return m_callService.myVideoEnabled();
    }

    int32_t CallViewModel::disconnectReason() {
        return m_callService.disconnectReason();
    }

    Microsoft::UI::Xaml::Visibility CallViewModel::visibleAlertMessage() {
        return m_visibleAlertMessage;
    }

    hstring CallViewModel::alertMessage() {
        return m_callService.alertMessage();
    }

    Windows::Foundation::IAsyncOperation<ConnectResult> CallViewModel::makeCall(bool videoCall, hstring const& inputDeviceUid, hstring const& outputDeviceUid) {
        co_return co_await m_callService.makeCall(m_settigService.userId(), m_settigService.accessToken(), videoCall, inputDeviceUid, outputDeviceUid);
    }

    void CallViewModel::endCall() {
        m_callService.endCall();
    }

    void CallViewModel::muteMyAudio() {
        m_callService.muteMyAudio();
    }

    void CallViewModel::pauseMyVideo() {
        m_callService.pauseMyVideo();
    }

    void CallViewModel::resumeMyVideo() {
        m_callService.resumeMyVideo();
    }
        
    void CallViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void CallViewModel::onTick(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_alertMessageShowTimer.Stop();
        m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;

        dispatchPropertyChanged(L"visibleAlertMessage");
    }

    void CallViewModel::serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            dispatchPropertyChanged(L"myVideoPaused");
            dispatchPropertyChanged(e.PropertyName());
        }
        else if (e.PropertyName() == L"alertMessage") {
            m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Visible;
            m_alertMessageShowTimer.Start();

            dispatchPropertyChanged(L"alertMessage");
            dispatchPropertyChanged(L"visibleAlertMessage");
        }
        else {
            dispatchPropertyChanged(e.PropertyName());
        }
    }
}
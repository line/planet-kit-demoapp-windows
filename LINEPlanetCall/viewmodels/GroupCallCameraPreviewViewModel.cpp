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

#include "GroupCallCameraPreviewViewModel.h"

#if __has_include("GroupCallCameraPreviewViewModel.g.cpp")
#include "GroupCallCameraPreviewViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    GroupCallCameraPreviewViewModel::GroupCallCameraPreviewViewModel(LINEPlanetCall::SettingService settingService, LINEPlanetCall::GroupCallService groupCallService, LINEPlanetCall::AudioDeviceService audioDeviceService) {
        m_settingService = settingService;
        m_groupCallService = groupCallService;
        m_audioDeviceService = audioDeviceService;

        m_groupCallEventToken = m_groupCallService.PropertyChanged({ this, &GroupCallCameraPreviewViewModel::groupCallServiceProperyChanged });

        m_alertMessageShowTimer.Interval(std::chrono::milliseconds{ Constants::alertTimer });
        m_alertMessageShowTimerToken = m_alertMessageShowTimer.Tick({ this, &GroupCallCameraPreviewViewModel::onTick });
    }

    GroupCallCameraPreviewViewModel::~GroupCallCameraPreviewViewModel() {
        m_alertMessageShowTimer.Tick(m_alertMessageShowTimerToken);
        m_groupCallService.PropertyChanged(m_groupCallEventToken);
    }

    event_token GroupCallCameraPreviewViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallCameraPreviewViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    bool GroupCallCameraPreviewViewModel::myMute() {
        return m_groupCallService.myMute();
    }

    bool GroupCallCameraPreviewViewModel::myVideoEnabled() {
        return m_groupCallService.myVideoEnabled();
    }

    Microsoft::UI::Xaml::Visibility GroupCallCameraPreviewViewModel::myVideoPaused() {
        return m_groupCallService.myVideoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    Microsoft::UI::Xaml::Visibility GroupCallCameraPreviewViewModel::visibleAlertMessage() {
        return m_visibleAlertMessage;
    }

    hstring GroupCallCameraPreviewViewModel::alertMessage() {
        return m_groupCallService.alertMessage();
    }

    Windows::Foundation::IAsyncOperation<ConnectResult> GroupCallCameraPreviewViewModel::joinConference() {
        co_return co_await m_groupCallService.joinConference(m_settingService.userId(), m_settingService.name(), m_settingService.accessToken(), m_audioDeviceService.inputDeviceUid(), m_audioDeviceService.outputDeviceUid());
    }

    void GroupCallCameraPreviewViewModel::muteMyAudio() {
        m_groupCallService.muteMyAudio();
    }

    void GroupCallCameraPreviewViewModel::pauseMyVideo() {
        m_groupCallService.pauseMyVideo();
    }

    void GroupCallCameraPreviewViewModel::resumeMyVideo() {
        m_groupCallService.resumeMyVideo();
    }
        
    void GroupCallCameraPreviewViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void GroupCallCameraPreviewViewModel::onTick(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_alertMessageShowTimer.Stop();
        m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;

        dispatchPropertyChanged(L"visibleAlertMessage");
    }

    void GroupCallCameraPreviewViewModel::groupCallServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
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
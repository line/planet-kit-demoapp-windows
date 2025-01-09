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

#pragma once

#include "GroupCallCameraPreviewViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct GroupCallCameraPreviewViewModel : GroupCallCameraPreviewViewModelT<GroupCallCameraPreviewViewModel> {
        GroupCallCameraPreviewViewModel(LINEPlanetCall::SettingService settingService, LINEPlanetCall::GroupCallService groupCallService, LINEPlanetCall::AudioDeviceService audioDeviceService);
        virtual ~GroupCallCameraPreviewViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        bool myMute();
        bool myVideoEnabled();

        Microsoft::UI::Xaml::Visibility myVideoPaused();

        Microsoft::UI::Xaml::Visibility visibleAlertMessage();
        hstring alertMessage();

        Windows::Foundation::IAsyncOperation<ConnectResult> joinConference();

        void muteMyAudio();
        void pauseMyVideo();
        void resumeMyVideo();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void onTick(IInspectable const& sender, IInspectable const& event);

        void groupCallServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::SettingService m_settingService{ nullptr };
        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };
        LINEPlanetCall::AudioDeviceService m_audioDeviceService{ nullptr };
        event_token m_groupCallEventToken;

        Microsoft::UI::Xaml::DispatcherTimer m_alertMessageShowTimer;
        event_token m_alertMessageShowTimerToken;
        Microsoft::UI::Xaml::Visibility m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct GroupCallCameraPreviewViewModel : GroupCallCameraPreviewViewModelT<GroupCallCameraPreviewViewModel, implementation::GroupCallCameraPreviewViewModel> {

    };
}
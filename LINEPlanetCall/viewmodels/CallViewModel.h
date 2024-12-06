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

#include "CallViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct CallViewModel : CallViewModelT<CallViewModel> {
        CallViewModel(LINEPlanetCall::SettingService settigService, LINEPlanetCall::CallService callService);
        virtual ~CallViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring peerId();
        void peerId(hstring const& peerId);

        ConnectState callState();

        Microsoft::UI::Xaml::Visibility myMute();
        Microsoft::UI::Xaml::Visibility myVideoPaused();

        bool myVideoEnabled();

        int32_t disconnectReason();

        Microsoft::UI::Xaml::Visibility visibleAlertMessage();
        hstring alertMessage();

        Windows::Foundation::IAsyncOperation<ConnectResult> makeCall(bool videoCall, hstring const& inputDeviceUid, hstring const& outputDeviceUid);
        void endCall();
        void muteMyAudio();

        void pauseMyVideo();
        void resumeMyVideo();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void onTick(IInspectable const& sender, IInspectable const& event);

        void serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::SettingService m_settigService{ nullptr };
        LINEPlanetCall::CallService m_callService{ nullptr };

        event_token m_settingServiceToken;
        event_token m_callServiceToken;

        Microsoft::UI::Xaml::DispatcherTimer m_alertMessageShowTimer;
        event_token m_alertMessageShowTimerToken;
        Microsoft::UI::Xaml::Visibility m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CallViewModel : CallViewModelT<CallViewModel, implementation::CallViewModel> {

    };
}
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

#include "GroupCallViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct GroupCallViewModel : GroupCallViewModelT<GroupCallViewModel> {
        GroupCallViewModel(LINEPlanetCall::GroupCallService groupCallService);
        virtual ~GroupCallViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        ConnectState groupCallState();
        int32_t disconnectReason();

        bool myMute();
        bool myVideoEnabled();

        Microsoft::UI::Xaml::Visibility myVideoPaused();

        Microsoft::UI::Xaml::Visibility visibleAlertMessage();
        hstring alertMessage();

        void leaveConference();

        void muteMyAudio();
        void pauseMyVideo();
        void resumeMyVideo();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void onTick(IInspectable const& sender, IInspectable const& event);

        void groupCallServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        Microsoft::UI::Xaml::DispatcherTimer m_alertMessageShowTimer;
        event_token m_alertMessageShowTimerToken;
        Microsoft::UI::Xaml::Visibility m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;

        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };
        event_token m_groupCallServiceToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct GroupCallViewModel : GroupCallViewModelT<GroupCallViewModel, implementation::GroupCallViewModel> {

    };
}
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

#include "CallMyViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct CallMyViewModel : CallMyViewModelT<CallMyViewModel> {
        CallMyViewModel(LINEPlanetCall::CallService callService);
        virtual ~CallMyViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        Microsoft::UI::Xaml::Visibility mute();
        Microsoft::UI::Xaml::Visibility pausedVideo();

        Microsoft::UI::Xaml::Media::Brush volumeBorderColor();

        bool videoEnabled();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
        void callMyMediaStatusPropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
        void serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::CallMyMediaStatusService m_callMyMediaStatusService{ nullptr };
        event_token m_callMyMediaStatusToken;

        LINEPlanetCall::CallService m_callService{ nullptr };
        event_token m_callServiceToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CallMyViewModel : CallMyViewModelT<CallMyViewModel, implementation::CallMyViewModel> {

    };
}
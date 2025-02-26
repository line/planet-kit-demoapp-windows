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

#include "CallMyMediaStatusService.g.h"
#include "MyMediaStatusListener.h"

#include "PlanetKitManager.h"

namespace winrt::LINEPlanetCall::implementation {
    struct CallMyMediaStatusService : CallMyMediaStatusServiceT<CallMyMediaStatusService> {
        CallMyMediaStatusService() = default;

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        void planetKitCall(PlanetKit::PlanetKitCallPtr planetKitCall);
        void clearPlanetKit();

        bool mute();
        void mute(bool mute);

        int32_t volume();

        bool videoEnabled();
        void videoEnabled(bool enabled);

        void onVolumeChanged(int32_t volume);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        PlanetKit::MyMediaStatusPtr m_myMediaStatus;

        MyMediaStatusListener m_myMediaStatusListener{ *this };

        bool m_mute = false;
        bool m_videoEnabled = false;

        int32_t m_volume = 0;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CallMyMediaStatusService : CallMyMediaStatusServiceT<CallMyMediaStatusService, implementation::CallMyMediaStatusService> {

    };
}
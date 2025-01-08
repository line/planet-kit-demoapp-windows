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

#include "AudioDevice.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct AudioDevice : AudioDeviceT<AudioDevice> {
        AudioDevice(PlanetKitAudioDeviceType type, hstring const& uid, hstring const& name);

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        PlanetKitAudioDeviceType type();
        hstring uid();
        hstring name();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        PlanetKitAudioDeviceType m_type;
        hstring m_uid;
        hstring m_name;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct AudioDevice : AudioDeviceT<AudioDevice, implementation::AudioDevice> {

    };
}
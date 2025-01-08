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

#include "AudioDeviceService.g.h"
#include "AudioDeviceEventListener.h"
#include "PlanetKitManager.h"

namespace winrt::LINEPlanetCall::implementation {
    struct AudioDeviceService : AudioDeviceServiceT<AudioDeviceService> {
        AudioDeviceService();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        Windows::Foundation::Collections::IObservableVector<AudioDevice> inputs();
        Windows::Foundation::Collections::IObservableVector<AudioDevice> outputs();

        int32_t selectedInputDeviceIndex();
        void selectedInputDeviceIndex(int32_t selectedIndex);

        int32_t selectedOutputDeviceIndex();
        void selectedOutputDeviceIndex(int32_t selectedIndex);

        hstring inputDeviceUid();
        hstring outputDeviceUid();

        void defaultAudioDeviceChanged(PlanetKitAudioDeviceType type);
        void addAudioDevice(PlanetKitAudioDeviceType type, hstring const& uid, hstring const& name);
        void removeAudioDevice(PlanetKitAudioDeviceType type, hstring const& uid);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        PlanetKit::AudioManagerPtr m_audioManager;
        AudioDeviceEventListener m_audioDeviceEventListener{ *this };

        int32_t m_selectedInputDevice = 0;
        int32_t m_selectedOutputDevice = 0;

        Windows::Foundation::Collections::IObservableVector<AudioDevice> m_audioDeviceInput = single_threaded_observable_vector<AudioDevice>();
        Windows::Foundation::Collections::IObservableVector<AudioDevice> m_audioDeviceOutput = single_threaded_observable_vector<AudioDevice>();
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct AudioDeviceService : AudioDeviceServiceT<AudioDeviceService, implementation::AudioDeviceService> {

    };
}
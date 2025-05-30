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

#include "AudioDevice.h"

#if __has_include("AudioDevice.g.cpp")
#include "AudioDevice.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    AudioDevice::AudioDevice(PlanetKitAudioDeviceType type, hstring const& uid, hstring const& name) {
        m_type = type;
        m_uid = uid;
        m_name = name;
    }

    event_token AudioDevice::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void AudioDevice::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    PlanetKitAudioDeviceType AudioDevice::type() {
        return m_type;
    }

    hstring AudioDevice::uid() {
        return m_uid;
    }

    hstring AudioDevice::name() {
        return m_name;
    }

    void AudioDevice::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
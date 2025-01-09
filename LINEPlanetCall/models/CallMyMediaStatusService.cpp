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

#include "CallMyMediaStatusService.h"

#if __has_include("CallMyMediaStatusService.g.cpp")
#include "CallMyMediaStatusService.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    event_token CallMyMediaStatusService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CallMyMediaStatusService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    void CallMyMediaStatusService::planetKitCall(PlanetKit::PlanetKitCallPtr planetKitCall) {
        m_myMediaStatus = planetKitCall->GetMyMediaStatus();
        m_myMediaStatus->Register(&m_myMediaStatusListener);
    }

    void CallMyMediaStatusService::clearPlanetKit() {
        m_myMediaStatus->Unregister();
        m_myMediaStatus = nullptr;
    }

    bool CallMyMediaStatusService::mute() {
        return m_mute;
    }

    void CallMyMediaStatusService::mute(bool mute) {
        if (m_mute != mute) {
            m_mute = mute;
            dispatchPropertyChanged(L"mute");
        }
    }

    int32_t CallMyMediaStatusService::volume() {
        return m_volume;
    }

    bool CallMyMediaStatusService::videoEnabled() {
        return m_videoEnabled;
    }

    void CallMyMediaStatusService::videoEnabled(bool enabled) {
        if (m_videoEnabled != enabled) {
            m_videoEnabled = enabled;
            dispatchPropertyChanged(L"videoEnabled");
        }
    }

    void CallMyMediaStatusService::onVolumeChanged(int32_t volume) {
        m_volume = volume;
        dispatchPropertyChanged(L"volume");
    }
    
    void CallMyMediaStatusService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
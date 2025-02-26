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

#include "CallPeerViewModel.h"

#if __has_include("CallPeerViewModel.g.cpp")
#include "CallPeerViewModel.g.cpp"
#endif

#include "UiUtils.h"

namespace winrt::LINEPlanetCall::implementation {
    CallPeerViewModel::CallPeerViewModel(LINEPlanetCall::CallService callService) {
        m_callService = callService;

        m_callServiceToken = m_callService.PropertyChanged({ this, &CallPeerViewModel::serviceProperyChanged });
    }

    CallPeerViewModel::~CallPeerViewModel() {
        m_callService.PropertyChanged(m_callServiceToken);
    }

    event_token CallPeerViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CallPeerViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring CallPeerViewModel::peerId() {
        return m_callService.peerId();
    }

    Microsoft::UI::Xaml::Visibility CallPeerViewModel::calling() {
        if (m_callService.callState() == ConnectState::connected) {
            return Microsoft::UI::Xaml::Visibility::Collapsed;
        }
        else {
            return Microsoft::UI::Xaml::Visibility::Visible;
        }
    }

    Microsoft::UI::Xaml::Visibility CallPeerViewModel::mute() {
        return m_callService.peerMute() == true ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }


    Microsoft::UI::Xaml::Visibility CallPeerViewModel::pausedVideo() {
        if (m_callService.callState() != ConnectState::connected) {
            return Microsoft::UI::Xaml::Visibility::Collapsed;
        }

        return m_callService.peerVideoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    Microsoft::UI::Xaml::Media::Brush CallPeerViewModel::volumeBorderColor() {
        static auto borderVolume = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(255, 6, 199, 85));
        static auto borderVolumeZero = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(0, 6, 199, 85));

        return m_callService.peerVolume() > Constants::volumeCheck ? borderVolume : borderVolumeZero;
    }

    bool CallPeerViewModel::videoEnabled() {
        return m_callService.peerVideoEnabled();
    }
    
    void CallPeerViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void CallPeerViewModel::serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"peerMute") {
            dispatchPropertyChanged(L"mute");
        }
        else if (e.PropertyName() == L"callState") {
            dispatchPropertyChanged(L"calling");
        }
        else if (e.PropertyName() == L"peerVideoEnabled") {
            dispatchPropertyChanged(L"pausedVideo");
            dispatchPropertyChanged(L"videoEnabled");
        }
        else if (e.PropertyName() == L"peerVolume") {
            dispatchPropertyChanged(L"volumeBorderColor");
        }
        else {
            dispatchPropertyChanged(e.PropertyName());
        }
    }
}
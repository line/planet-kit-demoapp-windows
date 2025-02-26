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

#include "CallMyViewModel.h"

#if __has_include("CallMyViewModel.g.cpp")
#include "CallMyViewModel.g.cpp"
#endif

#include "CallService.h"
#include "CallMyMediaStatusService.h"
#include "UiUtils.h"

namespace winrt::LINEPlanetCall::implementation {
    CallMyViewModel::CallMyViewModel(LINEPlanetCall::CallService callService) {
        m_callMyMediaStatusService = make<CallMyMediaStatusService>();

        m_callService = callService;
        auto planetKitcall = m_callService.as<CallService>()->planetKitCall();

        m_callMyMediaStatusService.as<CallMyMediaStatusService>()->planetKitCall(planetKitcall);

        m_callMyMediaStatusToken = m_callMyMediaStatusService.PropertyChanged({ this, &CallMyViewModel::callMyMediaStatusPropertyChanged });
        m_callServiceToken = m_callService.PropertyChanged({ this, &CallMyViewModel::serviceProperyChanged });

        // Initialize my media status.
        m_callMyMediaStatusService.mute(m_callService.myMute());
        m_callMyMediaStatusService.videoEnabled(m_callService.myVideoEnabled());
    }

    CallMyViewModel::~CallMyViewModel() {
        m_callService.PropertyChanged(m_callServiceToken);
        m_callMyMediaStatusService.PropertyChanged(m_callMyMediaStatusToken);
        m_callMyMediaStatusService.as<CallMyMediaStatusService>()->clearPlanetKit();
    }

    event_token CallMyViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CallMyViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Microsoft::UI::Xaml::Visibility CallMyViewModel::mute() {
        return m_callMyMediaStatusService.mute() == true ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }

    Microsoft::UI::Xaml::Visibility CallMyViewModel::pausedVideo() {
        if (m_callMyMediaStatusService) {
            return m_callMyMediaStatusService.videoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
        }
        else {
            return Microsoft::UI::Xaml::Visibility::Collapsed;
        }
    }

    Microsoft::UI::Xaml::Media::Brush CallMyViewModel::volumeBorderColor() {
        static auto borderVolume = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(255, 6, 199, 85));
        static auto borderVolumeZero = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(0, 6, 199, 85));

        return m_callMyMediaStatusService.volume() > Constants::volumeCheck ? borderVolume : borderVolumeZero;
    }

    bool CallMyViewModel::videoEnabled() {
        return m_callMyMediaStatusService.videoEnabled();
    }
    
    void CallMyViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void CallMyViewModel::callMyMediaStatusPropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"mute") {
            // property from CallMyMediaStatusService
            dispatchPropertyChanged(e.PropertyName());
        }
        else if (e.PropertyName() == L"videoEnabled") {
            // property from CallMyMediaStatusService
            dispatchPropertyChanged(L"pausedVideo");
            dispatchPropertyChanged(e.PropertyName());
        }
        else if (e.PropertyName() == L"volume") {
            dispatchPropertyChanged(L"volumeBorderColor");
        }
    }

    void CallMyViewModel::serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (m_callService.callState() != ConnectState::connected) {
            if (e.PropertyName() == L"myMute") {
                // property from CallService
                if (m_callService.callState() != ConnectState::connected) {
                    m_callMyMediaStatusService.mute(m_callService.myMute());
                }
            }
            else if (e.PropertyName() == L"myVideoEnabled") {
                // property from CallService
                if (m_callService.callState() != ConnectState::connected) {
                    m_callMyMediaStatusService.videoEnabled(m_callService.myVideoEnabled());
                }
            }
        }
    }
}
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

#include "VerifiedCallViewModel.h"

#if __has_include("VerifiedCallViewModel.g.cpp")
#include "VerifiedCallViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    VerifiedCallViewModel::VerifiedCallViewModel(LINEPlanetCall::CallService callService) {
        m_callService = callService;
        m_callServiceEventToken = m_callService.PropertyChanged({ this, &VerifiedCallViewModel::serviceProperyChanged });
    }

    VerifiedCallViewModel::~VerifiedCallViewModel() {
        m_callService.PropertyChanged(m_callServiceEventToken);
    }

    event_token VerifiedCallViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void VerifiedCallViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Microsoft::UI::Xaml::Visibility VerifiedCallViewModel::myMute() {
        return m_callService.myMute() == true ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }

    Microsoft::UI::Xaml::Visibility VerifiedCallViewModel::myVideoPaused() {
        return m_callService.myVideoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    bool VerifiedCallViewModel::myVideoEnabled() {
        return m_callService.myVideoEnabled();
    }

    void VerifiedCallViewModel::muteMyAudio() {
        m_callService.muteMyAudio();
    }

    void VerifiedCallViewModel::pauseMyVideo() {
        m_callService.pauseMyVideo();
    }

    void VerifiedCallViewModel::resumeMyVideo() {
        m_callService.resumeMyVideo();
    }
    
    void VerifiedCallViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void VerifiedCallViewModel::serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            dispatchPropertyChanged(L"myVideoPaused");
            dispatchPropertyChanged(L"myVideoEnabled");
        }
        else if (e.PropertyName() == L"myMute") {
            dispatchPropertyChanged(L"myMute");
        }
    }
}
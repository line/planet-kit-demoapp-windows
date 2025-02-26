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

#include "GroupCallParticipantViewModel.h"

#if __has_include("GroupCallParticipantViewModel.g.cpp")
#include "GroupCallParticipantViewModel.g.cpp"
#endif

#include "GroupCallParticipantService.h"

#include "UiUtils.h"

namespace winrt::LINEPlanetCall::implementation {
    GroupCallParticipantViewModel::GroupCallParticipantViewModel(LINEPlanetCall::GroupCallParticipantService groupCallParticipantService) {
        m_groupCallParticipantService = groupCallParticipantService;

        m_groupCallParticipantServiceEventToken = m_groupCallParticipantService.PropertyChanged({ this, &GroupCallParticipantViewModel::serviceProperyChanged });
    }

    GroupCallParticipantViewModel::~GroupCallParticipantViewModel() {
        m_groupCallParticipantService.PropertyChanged(m_groupCallParticipantServiceEventToken);
    }

    void GroupCallParticipantViewModel::registerVideoRender(VideoRender* videoRender) {
        m_groupCallParticipantService.as<GroupCallParticipantService>()->registerVideoRender(videoRender);
    }

    event_token GroupCallParticipantViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallParticipantViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    double_t GroupCallParticipantViewModel::itemWidth() {
        return m_groupCallParticipantService.itemWidth();
    }

    double_t GroupCallParticipantViewModel::itemHeight() {
        return m_groupCallParticipantService.itemHeight();
    }

    hstring GroupCallParticipantViewModel::userName() {
        return m_groupCallParticipantService.userName();
    }

    bool GroupCallParticipantViewModel::mute() {
        return m_groupCallParticipantService.mute();
    }

    bool GroupCallParticipantViewModel::videoEnabled() {
        return m_groupCallParticipantService.videoEnabled();
    }

    Microsoft::UI::Xaml::Media::Brush GroupCallParticipantViewModel::volumeBorderColor() {
        static auto borderVolume = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(255, 6, 199, 85));
        static auto borderVolumeZero = Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(0, 6, 199, 85));

        return m_groupCallParticipantService.volume() > Constants::volumeCheck ? borderVolume : borderVolumeZero;
    }

    Microsoft::UI::Xaml::Visibility GroupCallParticipantViewModel::videoPaused() {
        return m_groupCallParticipantService.videoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    void GroupCallParticipantViewModel::updateService(LINEPlanetCall::GroupCallParticipantService service) {
        if (m_groupCallParticipantService) {
            m_groupCallParticipantService.PropertyChanged(m_groupCallParticipantServiceEventToken);
        }

        m_groupCallParticipantService = service;
        m_groupCallParticipantServiceEventToken = m_groupCallParticipantService.PropertyChanged({ this, &GroupCallParticipantViewModel::serviceProperyChanged });

        dispatchPropertyChanged(L"userName");
        dispatchPropertyChanged(L"mute");
        dispatchPropertyChanged(L"videoPaused");
        dispatchPropertyChanged(L"videoEnabled");
    }

    void GroupCallParticipantViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void GroupCallParticipantViewModel::serviceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;
        dispatchPropertyChanged(e.PropertyName());

        if (e.PropertyName() == L"videoEnabled") {
            dispatchPropertyChanged(L"videoPaused");
        }
        else if (e.PropertyName() == L"volume") {
            dispatchPropertyChanged(L"volumeBorderColor");
        }
    }
}
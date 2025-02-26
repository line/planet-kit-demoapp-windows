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

#include "GroupCallParticipantService.h"

#if __has_include("GroupCallParticipantService.g.cpp")
#include "GroupCallParticipantService.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    GroupCallParticipantService::GroupCallParticipantService(hstring const& userId, hstring const& userName) {
        m_groupCallParticipantModel = GroupCallParticipantModel::Create(userId, userName, &m_groupCallParticipantEventListener);
    }

    GroupCallParticipantService::GroupCallParticipantService(PlanetKit::PeerPtr peer) {
        m_groupCallParticipantModel = GroupCallParticipantModel::Create(peer, &m_groupCallParticipantEventListener);
    }

    GroupCallParticipantService::~GroupCallParticipantService() {
        delete m_groupCallParticipantModel;
    }

    void GroupCallParticipantService::registerVideoRender(VideoRender* videoRender) {
        m_groupCallParticipantModel->registerVideoRender(videoRender);
    }

    event_token GroupCallParticipantService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallParticipantService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring GroupCallParticipantService::userId() {
        return m_groupCallParticipantModel->userId();
    }

    bool GroupCallParticipantService::mute() {
        return m_groupCallParticipantModel->mute();
    }

    bool GroupCallParticipantService::videoEnabled() {
        return m_groupCallParticipantModel->videoEnabled();
    }

    double_t GroupCallParticipantService::itemWidth() {
        return m_itemWidth;
    }

    void GroupCallParticipantService::itemWidth(double_t width) {
        if (m_itemWidth != width) {
            m_itemWidth = width;
            dispatchPropertyChanged(L"itemWidth");
        }
    }

    double_t GroupCallParticipantService::itemHeight() {
        return m_itemHeight;
    }

    void GroupCallParticipantService::itemHeight(double_t height) {
        if (m_itemHeight != height) {
            m_itemHeight = height;
            dispatchPropertyChanged(L"itemHeight");
        }
    }

    int32_t GroupCallParticipantService::volume() {
        return m_volume;
    }

    hstring GroupCallParticipantService::userName() {
        return m_groupCallParticipantModel->userName();
    }

    void GroupCallParticipantService::onMute(bool mute) {
        std::ignore = mute;
        dispatchPropertyChanged(L"mute");
    }

    void GroupCallParticipantService::onVideoEnabled(bool enabled) {
        std::ignore = enabled;
        dispatchPropertyChanged(L"videoEnabled");
    }

    void GroupCallParticipantService::onVolumeChanged(int32_t volume) {
        m_volume = volume;
        dispatchPropertyChanged(L"volume");
    }
    
    void GroupCallParticipantService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
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

#include "GroupCallParticipantService.g.h"
#include "GroupCallParticipantEventListener.h"
#include "GroupCallParticipantModel.h"

namespace winrt::LINEPlanetCall::implementation {
    struct GroupCallParticipantService : GroupCallParticipantServiceT<GroupCallParticipantService> {
        GroupCallParticipantService(hstring const& userId, hstring const& userName);
        GroupCallParticipantService(PlanetKit::PeerPtr peer);

        virtual ~GroupCallParticipantService();

        void registerVideoRender(VideoRender* videoRender);

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring userId();

        bool mute();
        bool videoEnabled();

        double_t itemWidth();
        void itemWidth(double_t width);

        double_t itemHeight();
        void itemHeight(double_t height);

        int32_t volume();

        hstring userName();

        void onMute(bool mute);
        void onVideoEnabled(bool enabled);
        void onVolumeChanged(int32_t volume);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        GroupCallParticipantEventListener m_groupCallParticipantEventListener{ *this };
        GroupCallParticipantModel* m_groupCallParticipantModel{ nullptr };

        double_t m_itemWidth = 326.0f;
        double_t m_itemHeight = 182.0f;

        int32_t m_volume = 0;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct GroupCallParticipantService : GroupCallParticipantServiceT<GroupCallParticipantService, implementation::GroupCallParticipantService> {

    };
}
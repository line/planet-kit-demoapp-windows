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

#include "winrt/LINEPlanetCall.h"

#include "PlanetKitManager.h"
#include "VideoRender.h"

namespace winrt::LINEPlanetCall::implementation {
    class ParticipantEventListener {
    public:
        virtual void onMute(bool mute) = 0;
        virtual void onVideoEnabled(bool enabled) = 0;
        virtual void onVolumeChanged(int32_t volume) = 0;
    };

    class GroupCallParticipantModel {
    public:
        static GroupCallParticipantModel* Create(hstring const& userId, hstring const& userName, ParticipantEventListener* eventListener);
        static GroupCallParticipantModel* Create(PlanetKit::PeerPtr peer, ParticipantEventListener* eventListener);

        virtual ~GroupCallParticipantModel() = default;

        virtual void registerVideoRender(VideoRender* videoRender) = 0;

        virtual hstring userName() = 0;
        virtual hstring userId() = 0;
        virtual bool mute() = 0;
        virtual bool videoEnabled() = 0;
    };
};
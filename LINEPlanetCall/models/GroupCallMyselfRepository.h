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

#include "GroupCallParticipantModel.h"
#include "MyMediaStatusListener.h"

namespace winrt::LINEPlanetCall::implementation {
    class GroupCallMyselfRepository : public GroupCallParticipantModel {
    public:
        GroupCallMyselfRepository(hstring const& userId, hstring const& userName, ParticipantEventListener* eventListener);
        virtual ~GroupCallMyselfRepository();

        void registerVideoRender(VideoRender* videoRender) override;

        hstring userName() override;
        hstring userId() override;
        bool mute() override;
        bool videoEnabled() override;

        void onVideoEnabled(bool videoEnabled);
        void onMute(bool mute);
        void onVolumeChanged(unsigned char volume);

    private:
        ParticipantEventListener* m_eventListener = nullptr;

        VideoRender* m_videoRender = nullptr;

        PlanetKit::MyMediaStatusPtr m_myMediaStatus;
        MyMediaStatusListener m_myMedisStatusListener{ this };

        hstring m_userName;
        hstring m_userId;
    };
}

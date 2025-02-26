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

#include "GroupCallParticipantModel.h"
#include "GroupCallMyselfRepository.h"

namespace winrt::LINEPlanetCall::implementation {
    GroupCallMyselfRepository::GroupCallMyselfRepository(hstring const& userId, hstring const& userName, ParticipantEventListener* eventListener) {
        m_userId = userId;
        m_userName = userName;
        m_eventListener = eventListener;        

        auto planetKitConference = PlanetKit::PlanetKitManager::GetInstance()->GetConferenceInstance();

        m_myMediaStatus = planetKitConference->GetMyMediaStatus();
        m_myMediaStatus->Register(&m_myMedisStatusListener);
    }

    GroupCallMyselfRepository::~GroupCallMyselfRepository() {
        m_myMediaStatus->Unregister();
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->DeregisterMyVideoReceiver(m_videoRender);
    }

    void GroupCallMyselfRepository::registerVideoRender(VideoRender* videoRender) {
        m_videoRender = videoRender;
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->RegisterMyVideoReceiver(m_videoRender);
    }

    hstring GroupCallMyselfRepository::userName() {
        return m_userName;
    }

    hstring GroupCallMyselfRepository::userId() {
        return m_userId;
    }

    bool GroupCallMyselfRepository::mute() {
        return m_myMediaStatus->IsAudioMuted();
    }

    bool GroupCallMyselfRepository::videoEnabled() {
        PlanetKit::VideoStatus videoStatus;
        m_myMediaStatus->GetVideoStatus(&videoStatus);

        return videoStatus.eVideoState == PlanetKit::EVideoState::PLNK_VIDEO_STATE_ENABLED ? true : false;
    }

    void GroupCallMyselfRepository::onVideoEnabled(bool videoEnabled) {
        m_eventListener->onVideoEnabled(videoEnabled);
    }

    void GroupCallMyselfRepository::onMute(bool mute) {
        m_eventListener->onMute(mute);
    }

    void GroupCallMyselfRepository::onVolumeChanged(unsigned char volume) {
        m_eventListener->onVolumeChanged((int32_t)volume);
    }
};


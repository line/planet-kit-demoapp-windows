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
#include "GroupCallPeerRepository.h"

namespace winrt::LINEPlanetCall::implementation {
    GroupCallPeerRepository::GroupCallPeerRepository(PlanetKit::PeerPtr peer, ParticipantEventListener* eventListener) {
        m_eventListener = eventListener;

        m_userId = peer->GetUserID()->GetID().c_str();

        if (peer->GetDisplayName().Size() > 0) {
            m_userName = peer->GetDisplayName().c_str();
        }

        m_peer = peer;

        m_peerControl = m_peer->CreatePeerControl();
        m_peerControl->Register(&m_peerEventListener);
    }

    GroupCallPeerRepository::~GroupCallPeerRepository() {
        m_peerControl->Unregister();
        m_peerControl->DeregisterReceiver(m_videoRender);
        m_videoRender = nullptr;
    }

    void GroupCallPeerRepository::registerVideoRender(VideoRender* videoRender) {
        m_videoRender = videoRender;
        m_videoRender->mirrorType(PlanetKit::EVideoMirrorType::PLNK_MIRROR_TYPE_UNMIRRORED);

        m_peerControl->RegisterReceiver(m_videoRender);
        m_peerControl->StartVideo(PlanetKit::NullOptional, PlanetKit::EVideoResolution::PLNK_VIDEO_RESOLUTION_RECOMMENDED);
    }

    hstring GroupCallPeerRepository::userId() {
        return m_userId;
    }

    bool GroupCallPeerRepository::mute() {
        return m_peer->IsAudioMuted();
    }

    bool GroupCallPeerRepository::videoEnabled() {
        return m_peer->GetVideoStatus().videoStatus.Value().eVideoState == PlanetKit::EVideoState::PLNK_VIDEO_STATE_ENABLED ? true : false;
    }

    hstring GroupCallPeerRepository::userName() {
        return m_userName;
    }

    void GroupCallPeerRepository::onInitialized() {
        onVideoEnabled(m_peer->GetVideoStatus().videoStatus.Value().eVideoState == PlanetKit::EVideoState::PLNK_VIDEO_STATE_ENABLED ? true : false);
        onMute(m_peer->IsAudioMuted());
    }

    void GroupCallPeerRepository::onMute(bool mute) {
        m_eventListener->onMute(mute);
    }

    void GroupCallPeerRepository::onVideoEnabled(bool enabled) {
        if (enabled == true) {
            m_peerControl->StartVideo(PlanetKit::NullOptional, PlanetKit::EVideoResolution::PLNK_VIDEO_RESOLUTION_RECOMMENDED);
        }
        else {
            m_peerControl->StopVideo();
        }

        m_eventListener->onVideoEnabled(enabled);
    }

    void GroupCallPeerRepository::onVolumeChanged(int32_t volume) {
        m_eventListener->onVolumeChanged(volume);
    }
};
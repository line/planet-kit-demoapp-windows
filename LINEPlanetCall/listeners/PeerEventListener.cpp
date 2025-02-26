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
#include "PeerEventListener.h"

#include "GroupCallParticipantModel.h"
#include "GroupCallPeerRepository.h"

namespace winrt::LINEPlanetCall::implementation {
    PeerEventListener::PeerEventListener(GroupCallPeerRepository* groupCallPeerRepository) {
        m_groupCallPeerRepository = groupCallPeerRepository;
    }

    PeerEventListener::~PeerEventListener() {

    }

    void PeerEventListener::OnInitialized(PlanetKit::PeerControlPtr pPeerControl, bool bResult) {
        std::ignore = pPeerControl;
        std::ignore = bResult;
        m_groupCallPeerRepository->onInitialized();
    }

    void PeerEventListener::OnMuted(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
        m_groupCallPeerRepository->onMute(true);
    }

    void PeerEventListener::OnUnmuted(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
        m_groupCallPeerRepository->onMute(false);
    }

    void PeerEventListener::OnVideoUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, const PlanetKit::VideoStatus& videoStatus) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;

        m_groupCallPeerRepository->onVideoEnabled(videoStatus.eVideoState == PlanetKit::EVideoState::PLNK_VIDEO_STATE_ENABLED ? true : false);
    }

    void PeerEventListener::OnScreenShareUpdated(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup, PlanetKit::EScreenShareState eState) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
        std::ignore = eState;
    }

    void PeerEventListener::OnSubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
    }

    void PeerEventListener::OnUnsubscribed(PlanetKit::PeerControlPtr pPeerControl, PlanetKit::SubgroupPtr pSubgroup) {
        std::ignore = pPeerControl;
        std::ignore = pSubgroup;
    }

    void PeerEventListener::OnDisconnected(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerEventListener::OnHold(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::WString& strHoldReason) {
        std::ignore = pPeerControl;
        std::ignore = strHoldReason;
    }

    void PeerEventListener::OnUnHold(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerEventListener::OnSetSharedContents(PlanetKit::PeerControlPtr pPeerControl, unsigned int unElapsedAfterSet, const void* pvData, unsigned int unDataSize) {
        std::ignore = pPeerControl;
        std::ignore = unElapsedAfterSet;
        std::ignore = pvData;
        std::ignore = unDataSize;
    }

    void PeerEventListener::OnUnsetSharedContents(PlanetKit::PeerControlPtr pPeerControl) {
        std::ignore = pPeerControl;
    }

    void PeerEventListener::OnPeerAudioDescriptionUpdated(PlanetKit::PeerControlPtr pPeerControl, const PlanetKit::PeerAudioDescription& sPeerAudioDescription) {
        std::ignore = pPeerControl;
        m_groupCallPeerRepository->onVolumeChanged(sPeerAudioDescription.ucVolume);
    }
};
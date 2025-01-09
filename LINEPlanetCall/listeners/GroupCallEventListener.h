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

#include "PlanetKitManager.h"

#include "GroupCallService.g.h"

namespace winrt::LINEPlanetCall::implementation {
    class GroupCallEventListener : public PlanetKit::IConferenceEvent {
    public:
        GroupCallEventListener(LINEPlanetCall::GroupCallService groupCallService);

        void OnConnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceConnectedParamPtr pParam) override;

        void OnDisconnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceDisconnectedParamPtr pParam) override;

        void OnPeerListUpdate(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferencePeerUpdateParamPtr pParam) override;

        void OnPeersVideoUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceVideoUpdatedParam* pParam) override;

        void OnPublicSubgroupUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceSubgroupUpdatePeerArray& arrSubgroupUpdatePeer) override;

        void OnReceivedShortData(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ShortDataParamPtr pParam) override;

        void OnReceivedPeersHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerHoldArray& arrPeerHold) override;

        void OnReceivedPeersUnhold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& arrPeer) override;

        void OnPeersMicMuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) override;

        void OnPeersMicUnmuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) override;

        void OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::PeerPtr pPeer, bool bEnableMute) override;

        void OnReceivedAppControlMessage(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const void* pMessage, int nMessageSize) override;

        void OnNetworkUnavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, int nWillDisconnectAfterSec) override;

        void OnNetworkReavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) override;

        void OnDataSessionIncoming(const PlanetKit::WStringOptional& strSubgroupName, PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) override;

        void OnPeerScreenShareUpdated(PlanetKit::ConferencePeerScreenShareUpdatedParam* pPeerScreenShareUpdateInfo) override;

        void OnException(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceExceptionArray& arrExceptionMessage) override;

        void OnMyVideoNoSource(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) override;

        void OnPeersAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, unsigned char ucAverageVolumeLevel, const PlanetKit::PeerAudioDescriptionArray& arrPeer) override;

        void OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::MyAudioDescription& sMyAudioDescription) override;

        void OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) override;

    private :
        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };
    };
}

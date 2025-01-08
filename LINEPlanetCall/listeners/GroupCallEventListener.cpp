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

#include "GroupCallEventListener.h"
#include "GroupCallService.h"

namespace winrt::LINEPlanetCall::implementation {
    GroupCallEventListener::GroupCallEventListener(LINEPlanetCall::GroupCallService groupCallService) {
        m_groupCallService = groupCallService;
    }

    void GroupCallEventListener::OnConnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceConnectedParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        std::ignore = pParam;

        m_groupCallService.onConnected();
    }

    void GroupCallEventListener::OnDisconnected(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceDisconnectedParamPtr pParam) {
        std::ignore = pPlanetKitConference;

        m_groupCallService.onDisconnected((int32_t)pParam->GetReason());
    }

    void GroupCallEventListener::OnPeerListUpdate(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        std::ignore = pPlanetKitConference;

        m_groupCallService.as<GroupCallService>()->onPeerListUpdated(pParam);
    }

    void GroupCallEventListener::OnPeersVideoUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ConferenceVideoUpdatedParam* pParam) {
        std::ignore = pPlanetKitConference;
        std::ignore = pParam;
    }

    void GroupCallEventListener::OnPublicSubgroupUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceSubgroupUpdatePeerArray& arrSubgroupUpdatePeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrSubgroupUpdatePeer;
    }

    void GroupCallEventListener::OnReceivedShortData(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::ShortDataParamPtr pParam) {
        std::ignore = pPlanetKitConference;
        std::ignore = pParam;
    }

    void GroupCallEventListener::OnReceivedPeersHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerHoldArray& arrPeerHold) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrPeerHold;
    }

    void GroupCallEventListener::OnReceivedPeersUnhold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& arrPeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrPeer;
    }

    void GroupCallEventListener::OnPeersMicMuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) {
        std::ignore = pPlanetKitConference;
        std::ignore = peerArray;
    }

    void GroupCallEventListener::OnPeersMicUnmuted(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::PeerArray& peerArray) {
        std::ignore = pPlanetKitConference;
        std::ignore = peerArray;
    }

    void GroupCallEventListener::OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, PlanetKit::PeerPtr pPeer, bool bEnableMute) {
        std::ignore = pPlanetKitConference;
        std::ignore = pPeer;
        std::ignore = bEnableMute;
    }

    void GroupCallEventListener::OnReceivedAppControlMessage(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const void* pMessage, int nMessageSize) {
        std::ignore = pPlanetKitConference;
        std::ignore = pMessage;
        std::ignore = nMessageSize;
    }

    void GroupCallEventListener::OnNetworkUnavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, int nWillDisconnectAfterSec) {
        std::ignore = pPlanetKitConference;
        std::ignore = nWillDisconnectAfterSec;
    }

    void GroupCallEventListener::OnNetworkReavailable(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }

    void GroupCallEventListener::OnDataSessionIncoming(const PlanetKit::WStringOptional& strSubgroupName, PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) {
        std::ignore = strSubgroupName;
        std::ignore = nStreamId;
        std::ignore = eType;
    }

    void GroupCallEventListener::OnPeerScreenShareUpdated(PlanetKit::ConferencePeerScreenShareUpdatedParam* pPeerScreenShareUpdateInfo) {
        std::ignore = pPeerScreenShareUpdateInfo;
    }

    void GroupCallEventListener::OnException(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::ConferenceExceptionArray& arrExceptionMessage) {
        std::ignore = pPlanetKitConference;
        std::ignore = arrExceptionMessage;
    }

    void GroupCallEventListener::OnMyVideoNoSource(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }

    void GroupCallEventListener::OnPeersAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, unsigned char ucAverageVolumeLevel, const PlanetKit::PeerAudioDescriptionArray& arrPeer) {
        std::ignore = pPlanetKitConference;
        std::ignore = ucAverageVolumeLevel;
        std::ignore = arrPeer;
    }

    void GroupCallEventListener::OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitConferencePtr pPlanetKitConference, const PlanetKit::MyAudioDescription& sMyAudioDescription) {
        std::ignore = pPlanetKitConference;
        std::ignore = sMyAudioDescription;
    }

    void GroupCallEventListener::OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitConferencePtr pPlanetKitConference) {
        std::ignore = pPlanetKitConference;
    }
}
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
#include "CallEventListener.h"

namespace winrt::LINEPlanetCall::implementation {
    CallEventListener::CallEventListener(LINEPlanetCall::CallService callService) {
        m_callService = callService;
    }

    void CallEventListener::OnConnected(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallConnectedParamPtr pConnectedParam) {
        std::ignore = pPlanetKitCall;
        std::ignore = pConnectedParam;

        m_callService.onConnected();
    }

    void CallEventListener::OnWaitAnswer(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnVerified(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallVerifiedParamPtr pVerifiedParam) {
        std::ignore = pPlanetKitCall;
        std::ignore = pVerifiedParam;
    }

    void CallEventListener::OnPreparationFinished(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnDisconnected(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallDisconnectedParamPtr pDisconnectedParam) {
        std::ignore = pPlanetKitCall;
        std::ignore = pDisconnectedParam;

        m_callService.onDisconnected((int32_t)pDisconnectedParam->GetReason());
    }

    void CallEventListener::OnPeerVideoPaused(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::EVideoPauseReason eVideoPauseReason) {
        std::ignore = pPlanetKitCall;
        std::ignore = eVideoPauseReason;

        m_callService.onPeerVideoEnabled(false);
    }

    void CallEventListener::OnPeerVideoResumed(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;

        m_callService.onPeerVideoEnabled(true);
    }

    void CallEventListener::OnReceivedShortData(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::ShortDataParamPtr pShortDataParam) {
        std::ignore = pPlanetKitCall;
        std::ignore = pShortDataParam;
    }

    void CallEventListener::OnVideoEnabledByPeer(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsSendingMyVideo) {
        std::ignore = pPlanetKitCall;
        std::ignore = bIsSendingMyVideo;
    }

    void CallEventListener::OnVideoDisabledByPeer(PlanetKit::PlanetKitCallPtr pPlanetKitCall, const PlanetKit::EMediaDisabledReason eReason) {
        std::ignore = pPlanetKitCall;
        std::ignore = eReason;
    }

    void CallEventListener::OnReceivedPeerHold(PlanetKit::PlanetKitCallPtr pPlanetKitCall, const PlanetKit::WString& strHoldReason) {
        std::ignore = pPlanetKitCall;
        std::ignore = strHoldReason;
    }

    void CallEventListener::OnReceivedPeerUnhold(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnNetworkUnavailable(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsPeer, int nDisconnectAfterSec) {
        std::ignore = pPlanetKitCall;
        std::ignore = bIsPeer;
        std::ignore = nDisconnectAfterSec;
    }

    void CallEventListener::OnNetworkReavailable(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsPeer) {
        std::ignore = pPlanetKitCall;
        std::ignore = bIsPeer;
    }

    void CallEventListener::OnPeerMicMuted(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
        m_callService.onPeerMute(true);
    }

    void CallEventListener::OnPeerMicUnmuted(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
        m_callService.onPeerMute(false);
    }

    void CallEventListener::OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bEnableMute) {
        std::ignore = pPlanetKitCall;
        std::ignore = bEnableMute;
    }

    void CallEventListener::OnPeerStartedScreenShare(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnPeerStoppedScreenShare(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bHasReason, int nUserReason) {
        std::ignore = pPlanetKitCall;
        std::ignore = bHasReason;
        std::ignore = nUserReason;
    }

    void CallEventListener::OnDataSessionIncoming(PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) {
        std::ignore = nStreamId;
        std::ignore = eType;
    }

    void CallEventListener::OnMyVideoNoSource(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnPeerAudioDescriptionUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, unsigned char ucAverageVolumeLevel) {
        std::ignore = pPlanetKitCall;
        m_callService.onPeerVolumeChanged((int32_t)ucAverageVolumeLevel);
    }

    void CallEventListener::OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, unsigned char ucAverageVolumeLevel) {
        std::ignore = pPlanetKitCall;
        std::ignore = ucAverageVolumeLevel;
    }

    void CallEventListener::OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitCallPtr pPlanetKitCall) {
        std::ignore = pPlanetKitCall;
    }

    void CallEventListener::OnRecordOnCloudUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::RecordOnCloudPtr pRecordOnCloud) {
        std::ignore = pPlanetKitCall;
        std::ignore = pRecordOnCloud;
    }
};

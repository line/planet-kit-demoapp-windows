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

#include "CallService.g.h"

namespace winrt::LINEPlanetCall::implementation {
    class CallEventListener : public PlanetKit::ICallEvent {
    public:
        CallEventListener(LINEPlanetCall::CallService callService);

        /**
         * Called when the call state is changed to connected.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param pConnectedParam Information about this call.
         */
        void OnConnected(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallConnectedParamPtr pConnectedParam) override;

        /**
         * Called when the call state is changed to wait answer.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnWaitAnswer(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when the call state is changed to verified.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param pVerifiedParam Information about this call.
         */
        void OnVerified(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallVerifiedParamPtr pVerifiedParam) override;

        /**
         * Called when the preparation is finished.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnPreparationFinished(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when the call state is changed to disconnected.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param pDisconnectedParam Information parameter
         */
        void OnDisconnected(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::CallDisconnectedParamPtr pDisconnectedParam) override;

        /**
         * Called when the peer's video is paused.
         * @param pPlanetKitCall PlanetKitCall object
         * @param eVideoPauseReason Paused reason
         */
        void OnPeerVideoPaused(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::EVideoPauseReason eVideoPauseReason) override;

        /**
         * Called when the peer's video is resumed.
         * @param pPlanetKitCall PlanetKitCall object
         */
        void OnPeerVideoResumed(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when short data is received.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param pShortDataParam
         */
        void OnReceivedShortData(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::ShortDataParamPtr pShortDataParam) override;


        /**
         * Called when video is enabled by the peer.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param bIsSendingMyVideo This indicates whether the local user's video is being sent to the peer. If this value is false, the local user's video is in a paused state.
         */
        void OnVideoEnabledByPeer(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsSendingMyVideo) override;

        /**
         * Called when video is disabled by the peer.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnVideoDisabledByPeer(PlanetKit::PlanetKitCallPtr pPlanetKitCall, const PlanetKit::EMediaDisabledReason eReason) override;

        /**
         * Called when the peer calls Hold().
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnReceivedPeerHold(PlanetKit::PlanetKitCallPtr pPlanetKitCall, const PlanetKit::WString& strHoldReason) override;

        /**
         * Called when the peer calls Unhold().
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnReceivedPeerUnhold(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when a network is unavailable.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param bIsPeer If true, the peer's is unavailable. Otherwise, the local user's network is unavailable.
         * @param nDisconnectAfterSec Seconds left until the call is disconnected. If the network becomes available again before nDisconnectAfterSec, the call will not be disconnected.
         */
        void OnNetworkUnavailable(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsPeer, int nDisconnectAfterSec) override;

        /**
         * Called when a network is available again.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param bIsPeer If true, the peer's network is available again. Otherwise, the local user's network is available again.
         */
        void OnNetworkReavailable(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bIsPeer) override;

        /**
         * Called when the peer has changed their microphone status to be muted.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnPeerMicMuted(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when the peer has changed their microphone status to be unmuted.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnPeerMicUnmuted(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when an audio mute request is received from the peer.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param bEnableMute
         */
        void OnReceivedMuteMyAudioRequest(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bEnableMute) override;

        /**
         * Called when the peer starts screen share.
         * @param pPlanetKitCall Call instance that the event was registered to.
         */
        void OnPeerStartedScreenShare(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when the peer stops screen share.
         * @param pPlanetKitCall Call instance that the event was registered to.
         * @param bHasReason If true, the peer stopped screen share with a user reason.
         * @param nUserReason Reason for the peer's stopping screen share. Valid if bHasReason is true.
         */
        void OnPeerStoppedScreenShare(PlanetKit::PlanetKitCallPtr pPlanetKitCall, bool bHasReason, int nUserReason) override;

        /**
         * Called when the peer sends data through a data session.
         * @param nStreamId Data session ID
         * @param eType Data session type
         */
        void OnDataSessionIncoming(PlanetKit::DataSessionStreamIdT nStreamId, PlanetKit::EDataSessionType eType) override;

        /**
         * Called when there is no video source for a certain period of time.
         * @param pPlanetKitCall 1-to-1 call instance
         */
        void OnMyVideoNoSource(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when a peer volume value is received for each interval that was registered at the time of initializing the call.
         * @param pPlanetKitCall 1-to-1 call instance
         * @param ucAverageVolumeLevel Volume[0:100]
         * @remark If the interval is 0, it does not occur.
         */
        void OnPeerAudioDescriptionUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, unsigned char ucAverageVolumeLevel) override;

        /**
         * Called when the local user's volume value is received for each interval that was registered at the time of initializing the call.
         * @param pPlanetKitCall 1-to-1 call instance
         * @param ucAverageVolumeLevel Volume[0:100]
         * @remark If the interval is 0, it does not occur.
         */
        void OnMyAudioDescriptionUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, unsigned char ucAverageVolumeLevel) override;

        /**
         * Called when the local user holds the call during screen share.
         */
        void OnMyScreenShareStoppedByHold(PlanetKit::PlanetKitCallPtr pPlanetKitCall) override;

        /**
         * Called when there is an event related to recording on the cloud.
         * @param pPlanetKitCall 1-to-1 call instance
         * @param pRecordOnCloud Instance of RecordOnCloud
         */
        void OnRecordOnCloudUpdated(PlanetKit::PlanetKitCallPtr pPlanetKitCall, PlanetKit::RecordOnCloudPtr pRecordOnCloud) override;

        private :
            LINEPlanetCall::CallService m_callService{ nullptr };
    };
};
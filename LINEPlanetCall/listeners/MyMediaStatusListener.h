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

#include "CallMyMediaStatusService.g.h"

namespace winrt::LINEPlanetCall::implementation {
    class GroupCallMyselfRepository;

    class MyMediaStatusListener : public PlanetKit::IMyMediaStatusEvent {
    public :
        MyMediaStatusListener(LINEPlanetCall::CallMyMediaStatusService callMyMediaStatusService);
        MyMediaStatusListener(GroupCallMyselfRepository* groupCallMyselfRepository);

        /// Called to notify the result of MyStatus::Register.
        void OnRegisterResult(PlanetKit::MyMediaStatusPtr pMyStatus, bool bResult) override;
        /// Called when the local user's video status is updated.
        void OnUpdateVideoStatus(PlanetKit::MyMediaStatusPtr pMyStatus, PlanetKit::VideoStatus sStatus) override;
        /// Called when the local user's video subgroup is updated.
        void OnUpdateVideoSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strSubgroupName) override;
        /// Called when the local user's screen share state is updated.
        void OnUpdateScreenShareState(PlanetKit::MyMediaStatusPtr pMyStatus, PlanetKit::EScreenShareState eState) override;
        /// Called when the local user's screen share subgroup is updated.
        void OnUpdateScreenShareSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strSubgroupName) override;
        /// Called when the local user's audio is muted.
        void OnMuted(PlanetKit::MyMediaStatusPtr pMyStatus) override;
        /// Called when the local user's audio is unmuted.
        void OnUnmuted(PlanetKit::MyMediaStatusPtr pMyStatus) override;
        /// Called when the local user's audio subgroup is updated.
        void OnAudioSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strDestinationSubgroup, const PlanetKit::WString& strTaggedSubgroup) override;

        /**
         * Called at each interval registered during the initialization of the call or the conference, providing the local user's volume value.
         * @param pMyStatus MyMediaStatus instance that the event was registered to.
         * @param sMyAudioDescription The local user's audio description.
         * @remark If the interval is 0, it does not occur.
         */
        void OnMyAudioDescriptionUpdated(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::MyAudioDescription& sMyAudioDescription) override;

    private :
        LINEPlanetCall::CallMyMediaStatusService m_callMyMediaStatusService{ nullptr };
        GroupCallMyselfRepository* m_groupCallMyselfRepository{ nullptr };
    };
};
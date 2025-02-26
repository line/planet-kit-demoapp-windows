
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
#include "MyMediaStatusListener.h"

#include "GroupCallMyselfRepository.h"

namespace winrt::LINEPlanetCall::implementation {
    MyMediaStatusListener::MyMediaStatusListener(LINEPlanetCall::CallMyMediaStatusService callMyMediaStatusService) {
        m_callMyMediaStatusService = callMyMediaStatusService;
    }

    MyMediaStatusListener::MyMediaStatusListener(GroupCallMyselfRepository* groupCallMyselfRepository) {
        m_groupCallMyselfRepository = groupCallMyselfRepository;
    }

    void MyMediaStatusListener::OnRegisterResult(PlanetKit::MyMediaStatusPtr pMyStatus, bool bResult) {
        std::ignore = pMyStatus;
        std::ignore = bResult;
    }

    void MyMediaStatusListener::OnUpdateVideoStatus(PlanetKit::MyMediaStatusPtr pMyStatus, PlanetKit::VideoStatus sStatus) {
        std::ignore = pMyStatus;

        bool videoEnabled = sStatus.eVideoState == PlanetKit::EVideoState::PLNK_VIDEO_STATE_ENABLED ? true : false;

        if (m_callMyMediaStatusService) {
            m_callMyMediaStatusService.videoEnabled(videoEnabled);
        }
        else if (m_groupCallMyselfRepository) {
            m_groupCallMyselfRepository->onVideoEnabled(videoEnabled);
        }
    }

    void MyMediaStatusListener::OnUpdateVideoSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strSubgroupName) {
        std::ignore = pMyStatus;
        std::ignore = strSubgroupName;
    }

    void MyMediaStatusListener::OnUpdateScreenShareState(PlanetKit::MyMediaStatusPtr pMyStatus, PlanetKit::EScreenShareState eState) {
        std::ignore = pMyStatus;
        std::ignore = eState;
    }

    void MyMediaStatusListener::OnUpdateScreenShareSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strSubgroupName) {
        std::ignore = pMyStatus;
        std::ignore = strSubgroupName;
    }

    void MyMediaStatusListener::OnMuted(PlanetKit::MyMediaStatusPtr pMyStatus) {
        std::ignore = pMyStatus;
        if (m_callMyMediaStatusService) {
            m_callMyMediaStatusService.mute(true);
        }
        else if (m_groupCallMyselfRepository) {
            m_groupCallMyselfRepository->onMute(true);
        }
    }

    void MyMediaStatusListener::OnUnmuted(PlanetKit::MyMediaStatusPtr pMyStatus) {
        std::ignore = pMyStatus;
        if (m_callMyMediaStatusService) {
            m_callMyMediaStatusService.mute(false);
        }
        else if (m_groupCallMyselfRepository) {
            m_groupCallMyselfRepository->onMute(false);
        }
    }

    void MyMediaStatusListener::OnAudioSubgroup(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::WString& strDestinationSubgroup, const PlanetKit::WString& strTaggedSubgroup) {
        std::ignore = pMyStatus;
        std::ignore = strDestinationSubgroup;
        std::ignore = strTaggedSubgroup;
    }

    void MyMediaStatusListener::OnMyAudioDescriptionUpdated(PlanetKit::MyMediaStatusPtr pMyStatus, const PlanetKit::MyAudioDescription& sMyAudioDescription) {
        std::ignore = pMyStatus;
        std::ignore = sMyAudioDescription;

        if (m_callMyMediaStatusService) {
            m_callMyMediaStatusService.onVolumeChanged((int32_t)sMyAudioDescription.ucVolume);
        }
        else if (m_groupCallMyselfRepository) {
            m_groupCallMyselfRepository->onVolumeChanged(sMyAudioDescription.ucVolume);
        }
        
    }
}
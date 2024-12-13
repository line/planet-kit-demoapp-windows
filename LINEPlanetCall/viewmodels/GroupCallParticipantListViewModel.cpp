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

#include "GroupCallParticipantListViewModel.h"

#if __has_include("GroupCallParticipantListViewModel.g.cpp")
#include "GroupCallParticipantListViewModel.g.cpp"
#endif

#include "GroupCallParticipantService.h"

namespace winrt::LINEPlanetCall::implementation {
    GroupCallParticipantListViewModel::GroupCallParticipantListViewModel(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;

        // initialize peer list
        auto settingService = m_parentWindow.settingService();
        auto myselfService = make<GroupCallParticipantService>(settingService.userId(), settingService.name());

        m_groupCallParticipantList.Append(myselfService);

        m_groupCallService = m_parentWindow.groupCallService();
        m_groupCallService.as<GroupCallService>()->registerParticipantUpdatedEvent(&m_groupCallPeerListUpdatedEventListener);

        m_disconnectedParticipantShowTimer.Interval(std::chrono::milliseconds{ Constants::alertTimer });
        m_disconnectedParticipantShowTimerToken = m_disconnectedParticipantShowTimer.Tick({ this, &GroupCallParticipantListViewModel::onTick });
    }

    GroupCallParticipantListViewModel::~GroupCallParticipantListViewModel() {
        m_disconnectedParticipantShowTimer.Tick(m_disconnectedParticipantShowTimerToken);
        m_groupCallService.as<GroupCallService>()->unregisterParticipantUpdatedEvent();
    }

    void GroupCallParticipantListViewModel::onPeerListUpdated(PlanetKit::ConferencePeerUpdateParamPtr pParam) {
        // append and remove peer
        auto& removedPeers = pParam->GetRemovedPeer();

        for (int32_t idx = 0; idx < removedPeers.Size(); ++idx) {
            auto peer = removedPeers[idx];
            hstring userId = peer->GetUserID()->GetID().c_str();

            removeAt(userId);
        }

        auto itemWidth = m_groupCallParticipantList.GetAt(0).itemWidth();
        auto itemHeight = m_groupCallParticipantList.GetAt(0).itemHeight();

        auto& addedPeers = pParam->GetAddedPeer();
        for (int32_t idx = 0; idx < addedPeers.Size(); ++idx) {
            auto participantService = make<GroupCallParticipantService>(addedPeers[idx]);
            participantService.itemWidth(itemWidth);
            participantService.itemHeight(itemHeight);
            m_groupCallParticipantList.Append(participantService);
        }

        dispatchPropertyChanged(L"userCount");
    }

    event_token GroupCallParticipantListViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallParticipantListViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Windows::Foundation::Collections::IObservableVector<LINEPlanetCall::GroupCallParticipantService> GroupCallParticipantListViewModel::groupCallParticipantList() {
        return m_groupCallParticipantList;
    }

    hstring GroupCallParticipantListViewModel::disconnetedParticipantMessage() {
        return m_disconnetedParticipantMessage;
    }

    Microsoft::UI::Xaml::Visibility GroupCallParticipantListViewModel::visibleDisconnectedPartitipant() {
        return m_visibleDisconnectedPartitipant;
    }

    int32_t GroupCallParticipantListViewModel::userCount() {
        return (int32_t)m_groupCallParticipantList.Size();
    }

    void GroupCallParticipantListViewModel::resizeItem(double_t width, double_t height) {
        for (auto item : m_groupCallParticipantList) {
            item.itemWidth(width);
            item.itemHeight(height);
        }
    }

    void GroupCallParticipantListViewModel::clear() {
        m_groupCallParticipantList.Clear();
        m_groupCallService.as<GroupCallService>()->unregisterParticipantUpdatedEvent();
    }
    
    void GroupCallParticipantListViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void GroupCallParticipantListViewModel::onTick(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_disconnectedParticipantShowTimer.Stop();
        m_visibleDisconnectedPartitipant = Microsoft::UI::Xaml::Visibility::Collapsed;

        dispatchPropertyChanged(L"disconnetedParticipantMessage");
        dispatchPropertyChanged(L"visibleDisconnectedPartitipant");
    }

    void GroupCallParticipantListViewModel::removeAt(hstring const& removeUserId) {
        for (uint32_t index = 0; index < m_groupCallParticipantList.Size(); ++index) {
            auto participant = m_groupCallParticipantList.GetAt(index);
            if (participant.userId() == removeUserId) {
                m_groupCallParticipantList.RemoveAt(index);

                Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
                std::wstring message = resourceLoader.GetString(L"lp_demoapp_group_scenarios_basic_inacall_toast/Text").c_str();
                message.replace(message.find(replaceNameString), replaceNameString.size(), participant.userName());

                m_disconnetedParticipantMessage = message.c_str();
                m_visibleDisconnectedPartitipant = Microsoft::UI::Xaml::Visibility::Visible;
                dispatchPropertyChanged(L"disconnetedParticipantMessage");
                dispatchPropertyChanged(L"visibleDisconnectedPartitipant");

                m_disconnectedParticipantShowTimer.Start();

                break;
            }
        }
    }
}
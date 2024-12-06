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

#include "GroupCallParticipantListViewModel.g.h"
#include "GroupCallPeerListUpdatedEventListener.h"

#include <string>

namespace winrt::LINEPlanetCall::implementation {
    struct GroupCallParticipantListViewModel : GroupCallParticipantListViewModelT<GroupCallParticipantListViewModel> {
        GroupCallParticipantListViewModel(LINEPlanetCall::MainWindow parentWindow);
        virtual ~GroupCallParticipantListViewModel();

        void onPeerListUpdated(PlanetKit::ConferencePeerUpdateParamPtr pParam);

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        Windows::Foundation::Collections::IObservableVector<LINEPlanetCall::GroupCallParticipantService> groupCallParticipantList();

        hstring disconnetedParticipantMessage();
        Microsoft::UI::Xaml::Visibility visibleDisconnectedPartitipant();

        int32_t userCount();

        void resizeItem(double_t width, double_t height);
        void clear();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void onTick(IInspectable const& sender, IInspectable const& event);

        void removeAt(hstring const& removeUserId);
    
    private:
        const std::wstring replaceNameString{ L"{{User name}}" };

        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        GroupCallPeerListUpdatedEventListener m_groupCallPeerListUpdatedEventListener{ *this };

        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };
        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };

        hstring m_disconnetedParticipantMessage;
        Microsoft::UI::Xaml::Visibility m_visibleDisconnectedPartitipant = Microsoft::UI::Xaml::Visibility::Collapsed;

        Microsoft::UI::Xaml::DispatcherTimer m_disconnectedParticipantShowTimer;
        event_token m_disconnectedParticipantShowTimerToken;

        Windows::Foundation::Collections::IObservableVector<LINEPlanetCall::GroupCallParticipantService> m_groupCallParticipantList = single_threaded_observable_vector<LINEPlanetCall::GroupCallParticipantService>();
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct GroupCallParticipantListViewModel : GroupCallParticipantListViewModelT<GroupCallParticipantListViewModel, implementation::GroupCallParticipantListViewModel> {

    };
}
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

#include "GroupCallRoomNameViewModel.h"

#if __has_include("GroupCallRoomNameViewModel.g.cpp")
#include "GroupCallRoomNameViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    GroupCallRoomNameViewModel::GroupCallRoomNameViewModel(LINEPlanetCall::GroupCallService groupCallService) {
        m_groupCallService = groupCallService;
    }

    event_token GroupCallRoomNameViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallRoomNameViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring GroupCallRoomNameViewModel::roomId() {
        return m_groupCallService.roomId();
    }

    void GroupCallRoomNameViewModel::roomId(hstring const& roomId) {
        m_groupCallService.roomId(roomId);
    }
    
    void GroupCallRoomNameViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
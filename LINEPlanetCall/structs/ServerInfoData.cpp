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

#include "ServerInfoData.h"

#if __has_include("ServerInfoData.g.cpp")
#include "ServerInfoData.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    ServerInfoData::ServerInfoData() {

    }

    event_token ServerInfoData::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void ServerInfoData::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring ServerInfoData::serviceId() {
        return m_serviceId;
    }

    void ServerInfoData::serviceId(hstring const& serviceId) {
        m_serviceId = serviceId;
    }

    hstring ServerInfoData::serverUrl() {
        return m_serverUrl;
    }

    void ServerInfoData::serverUrl(hstring const& serverUrl) {
        m_serverUrl = serverUrl;
    }

    hstring ServerInfoData::saturnServerUrl() {
        return m_saturnServerUrl;
    }

    void ServerInfoData::saturnServerUrl(hstring const& saturnServerUrl) {
        m_saturnServerUrl = saturnServerUrl;
    }
}
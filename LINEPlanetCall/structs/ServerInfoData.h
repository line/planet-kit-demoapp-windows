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

#include "ServerInfoData.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct ServerInfoData : ServerInfoDataT<ServerInfoData> {
        ServerInfoData();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring serviceId();
        void serviceId(hstring const& serviceId);

        hstring serverUrl();
        void serverUrl(hstring const& serverUrl);

        hstring saturnServerUrl();
        void saturnServerUrl(hstring const& saturnServerUrl);

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        hstring m_serviceId = L"line-planet-call";
        hstring m_serverUrl = L"https://voipnx-as.line-apps.com";
        hstring m_saturnServerUrl = L"https://voipnx-saturn.line-apps.com";
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct ServerInfoData : ServerInfoDataT<ServerInfoData, implementation::ServerInfoData> {

    };
}
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

#include "AppServerController.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct AppServerController : AppServerControllerT<AppServerController> {
        AppServerController();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        void stop();

        Windows::Foundation::IAsyncOperation<LINEPlanetCall::RegisterUserResult> registerUser(hstring userId);
        Windows::Foundation::IAsyncOperation<bool> registerDevice(hstring accessToken, hstring appVersion);
        Windows::Foundation::IAsyncOperation<hstring> accessToken(hstring userId);
        Windows::Foundation::IAsyncOperation<NotificationResult> notification(hstring accessToken, hstring appVersion);
        void stopNotification();

    private :
        void get(hstring const& uri, hstring const& auth, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode);

        void post(hstring const& uri, hstring const& requestData, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode);
        void post(hstring const& uri, hstring const& auth, hstring const& requestData, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode);

        bool isSuccessStatusCode(const winrt::Windows::Web::Http::HttpStatusCode statusCode);
        bool updateNotificationToken(hstring const& accessToken, hstring const& appVersion);

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        
        Windows::Web::Http::HttpClient m_httpClient;
        Windows::Foundation::IAsyncOperationWithProgress<Windows::Web::Http::HttpResponseMessage, Windows::Web::Http::HttpProgress> m_progress{ nullptr };

        LINEPlanetCall::ServerInfoData m_serverInfo;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct AppServerController : AppServerControllerT<AppServerController, implementation::AppServerController> {

    };
}
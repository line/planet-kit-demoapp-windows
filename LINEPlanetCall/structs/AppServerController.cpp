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

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Data.Json.h>

#include "AppServerController.h"

#if __has_include("AppServerController.g.cpp")
#include "AppServerController.g.cpp"
#endif

#include "AppServerAPI.h"

namespace winrt::LINEPlanetCall::implementation {
    AppServerController::AppServerController() {

    }

    event_token AppServerController::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void AppServerController::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    void AppServerController::stop() {
        stopNotification();
    }

    Windows::Foundation::IAsyncOperation<LINEPlanetCall::RegisterUserResult> AppServerController::registerUser(hstring userId) {
        hstring uri = m_serverInfo.serverUrl() + AppServerAPI::API::registerUser;

        Windows::Web::Http::HttpStatusCode statusCode;
        hstring response;

        Windows::Data::Json::JsonObject data;

        data.SetNamedValue(AppServerAPI::Key::userId, Windows::Data::Json::JsonValue::CreateStringValue(userId));
        data.SetNamedValue(AppServerAPI::Key::serviceId, Windows::Data::Json::JsonValue::CreateStringValue(m_serverInfo.serviceId()));
        data.SetNamedValue(AppServerAPI::Key::region, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::API::regionKey));
        data.SetNamedValue(AppServerAPI::Key::apiKey, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::API::registerUserApiKey));

        LINEPlanetCall::RegisterUserResult registerResult{ AccountStatus::unregistered };

        post(uri, data.ToString(), response, statusCode);
        OutputDebugStringW(response.c_str());

        if (true == isSuccessStatusCode(statusCode)) {
            try {
                Windows::Data::Json::JsonObject responseResult = Windows::Data::Json::JsonObject::Parse(response);
                
                if (responseResult.TryLookup(AppServerAPI::Key::status)) {
                    if (responseResult.Lookup(AppServerAPI::Key::status).GetString() == AppServerAPI::Key::success) {
                        registerResult.accountStatus = AccountStatus::registered;

                        if (responseResult.TryLookup(AppServerAPI::Key::data)) {
                            auto jsonResultData = responseResult.Lookup(AppServerAPI::Key::data).GetObjectW();
                            if (jsonResultData && jsonResultData.TryLookup(AppServerAPI::Key::accessToken)) {
                                registerResult.accessToken = jsonResultData.Lookup(AppServerAPI::Key::accessToken).GetString();
                            }
                        }
                    }
                    else {
                        registerResult.accountStatus = AccountStatus::unregistered;
                    }
                }
                else {
                    
                }
            }
            catch (winrt::hresult_error const&) {
                
            }
        }
        else if (statusCode == Windows::Web::Http::HttpStatusCode::Conflict) {
            // This user id is already used by other.
            registerResult.accountStatus = AccountStatus::conflict;
        }

        co_return registerResult;
    }

    Windows::Foundation::IAsyncOperation<bool> AppServerController::registerDevice(hstring accessToken, hstring appVersion) {
        hstring uri = m_serverInfo.serverUrl() + AppServerAPI::API::registerDevice;

        Windows::Data::Json::JsonObject data;

        data.SetNamedValue(AppServerAPI::Key::appType, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::AppType));
        data.SetNamedValue(AppServerAPI::Key::appVer, Windows::Data::Json::JsonValue::CreateStringValue(appVersion));

        bool result = false;

        Windows::Web::Http::HttpStatusCode statusCode;
        hstring response;

        post(uri, accessToken, data.ToString(), response, statusCode);

        if (true == isSuccessStatusCode(statusCode)) {
            try {
                Windows::Data::Json::JsonObject responseResult = Windows::Data::Json::JsonObject::Parse(response);

                if (responseResult.TryLookup(AppServerAPI::Key::status)) {
                    if (responseResult.Lookup(AppServerAPI::Key::status).GetString() == AppServerAPI::Key::success) {
                        result = true;
                    }
                    else {
                        result = false;
                    }
                }
                else {

                }
            }
            catch (winrt::hresult_error const&) {

            }
        }

        co_return result;
    }

    Windows::Foundation::IAsyncOperation<hstring> AppServerController::accessToken(hstring accessToken) {
        hstring uri = m_serverInfo.serverUrl() + AppServerAPI::API::accessToken;
        
        hstring resultAccessToken;

        hstring response;
        winrt::Windows::Web::Http::HttpStatusCode statusCode;

        get(uri, accessToken, response, statusCode);

        if (false == isSuccessStatusCode(statusCode)) {
            co_return resultAccessToken;
        }

        try {
            Windows::Data::Json::JsonObject responseResult = Windows::Data::Json::JsonObject::Parse(response);

            if (responseResult.TryLookup(AppServerAPI::Key::status)) {
                if (responseResult.Lookup(AppServerAPI::Key::status).GetString() == AppServerAPI::Key::success) {
                    if (responseResult.TryLookup(AppServerAPI::Key::data)) {
                        auto gwAccessToken = responseResult.Lookup(AppServerAPI::Key::data).GetObjectW();

                        if (gwAccessToken.TryLookup(AppServerAPI::Key::gwAccessToken)) {
                            resultAccessToken = gwAccessToken.Lookup(AppServerAPI::Key::gwAccessToken).GetString().c_str();
                        }
                    }
                }
                else {
                    // do nothing
                }
            }
        }
        catch (winrt::hresult_error const&) {

        }

        co_return resultAccessToken;
    }

    Windows::Foundation::IAsyncOperation<NotificationResult> AppServerController::notification(hstring accessToken, hstring appVersion) {
        if (updateNotificationToken(accessToken, appVersion) == false) {
            NotificationResult result{ .statusCode = Windows::Web::Http::HttpStatusCode::BadRequest };
            co_return result;
        }

        hstring uri = m_serverInfo.serverUrl() + AppServerAPI::API::notification;

        hstring response;
        winrt::Windows::Web::Http::HttpStatusCode statusCode;

        do {
            get(uri, accessToken, response, statusCode);

            if (statusCode == winrt::Windows::Web::Http::HttpStatusCode::ServiceUnavailable) {
                // this is time over.
                // try again.
                Sleep(5000);
                continue;
            }
            else if (false == isSuccessStatusCode(statusCode)) {
                NotificationResult result{ .statusCode = statusCode };
                co_return result;
            }

            try {
                auto responseResult = Windows::Data::Json::JsonObject::Parse(response);

                if (responseResult.TryLookup(AppServerAPI::Key::status)) {
                    if (responseResult.Lookup(AppServerAPI::Key::status).GetString() == AppServerAPI::Key::success) {
                        if (responseResult.TryLookup(AppServerAPI::Key::data)) {
                            NotificationResult result{ .statusCode = Windows::Web::Http::HttpStatusCode::Ok };
                            auto notificationData = responseResult.Lookup(AppServerAPI::Key::data).GetObjectW();

                            result.peerId = notificationData.Lookup(AppServerAPI::Key::callerUserId).GetString();
                            result.ccParam = notificationData.Lookup(AppServerAPI::Key::ccParam).GetString();

                            if (notificationData.Lookup(AppServerAPI::Key::appCallType).GetString() == L"a" ||
                                notificationData.Lookup(AppServerAPI::Key::appCallType).GetString() == L"A") {
                                result.mediaType = PlanetKitMediaType::audio;
                            }
                            else {
                                result.mediaType = PlanetKitMediaType::audioVideo;
                            }

                            co_return result;
                        }
                    }
                    else {
                        // do nothing
                    }
                }
            }
            catch (winrt::hresult_error const&) {

            }
        } while (statusCode != winrt::Windows::Web::Http::HttpStatusCode::None);

        NotificationResult result{ .statusCode = statusCode };

        m_progress = nullptr;

        co_return result;
    }

    void AppServerController::stopNotification() {
        if (m_progress) {
            m_progress.Cancel();
            m_progress = nullptr;
        }
    }

    void AppServerController::get(hstring const& uri, hstring const& auth, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode) {
        statusCode = Windows::Web::Http::HttpStatusCode::None;

        Windows::Foundation::Uri requestUri{ uri };
        Windows::Web::Http::HttpClient httpClient;

        if (auth.size() > 0) {
            Windows::Web::Http::Headers::HttpCredentialsHeaderValue header(L"Bearer", auth);
            httpClient.DefaultRequestHeaders().Authorization(header);
        }

        Windows::Web::Http::HttpResponseMessage httpResponseMessage;

        try {
            m_progress = httpClient.GetAsync(requestUri);
            m_progress.get();

            httpResponseMessage = m_progress.GetResults();
            m_progress = nullptr;

            statusCode = httpResponseMessage.StatusCode();

            if (httpResponseMessage.IsSuccessStatusCode()) {
                result = httpResponseMessage.Content().ReadAsStringAsync().get();
            }
            else {
                result = httpResponseMessage.ToString();
            }
        }
        catch (winrt::hresult_error const& ex) {
            // error message
            result = ex.message();
        }
    }

    void AppServerController::post(hstring const& uri, hstring const& requestData, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode) {
        Windows::Foundation::Uri requestUri{ uri };

        Windows::Web::Http::HttpStringContent content(
            requestData,
            Windows::Storage::Streams::UnicodeEncoding::Utf8,
            L"application/json"
        );

        Windows::Web::Http::HttpClient httpClient;
        Windows::Web::Http::HttpResponseMessage httpResponseMessage;

        try {
            m_progress = httpClient.PostAsync(requestUri, content);
            m_progress.get();

            httpResponseMessage = m_progress.GetResults();
            m_progress = nullptr;

            statusCode = httpResponseMessage.StatusCode();

            if (httpResponseMessage.IsSuccessStatusCode()) {
                result = httpResponseMessage.Content().ReadAsStringAsync().get();
            }
            else {
                result = httpResponseMessage.ToString();
            }
        }
        catch (winrt::hresult_error const& ex) {
            // error message
            result = ex.message();
        }
    }

    void AppServerController::post(hstring const& uri, hstring const& auth, hstring const& requestData, hstring& result, winrt::Windows::Web::Http::HttpStatusCode& statusCode) {
        Windows::Foundation::Uri requestUri{ uri };

        Windows::Web::Http::HttpStringContent content(
            requestData,
            Windows::Storage::Streams::UnicodeEncoding::Utf8,
            L"application/json"
        );

        Windows::Web::Http::HttpClient httpClient;

        if (auth.size() > 0) {
            Windows::Web::Http::Headers::HttpCredentialsHeaderValue header(L"Bearer", auth);
            httpClient.DefaultRequestHeaders().Authorization(header);
        }

        Windows::Web::Http::HttpResponseMessage httpResponseMessage;

        try {
            m_progress = httpClient.PostAsync(requestUri, content);
            m_progress.get();

            httpResponseMessage = m_progress.GetResults();
            m_progress = nullptr;

            statusCode = httpResponseMessage.StatusCode();

            if (httpResponseMessage.IsSuccessStatusCode()) {
                result = httpResponseMessage.Content().ReadAsStringAsync().get();
            }
            else {
                result = httpResponseMessage.ToString();
            }
        }
        catch (winrt::hresult_error const& ex) {
            // error message
            result = ex.message();
        }
    }

    bool AppServerController::isSuccessStatusCode(const winrt::Windows::Web::Http::HttpStatusCode statusCode) {
        if (statusCode >= winrt::Windows::Web::Http::HttpStatusCode::Ok && statusCode < winrt::Windows::Web::Http::HttpStatusCode::MultipleChoices) {
            return true;
        }
        return false;
    }

    bool AppServerController::updateNotificationToken(hstring const& accessToken, hstring const& appVersion) {
        hstring uri = m_serverInfo.serverUrl() + AppServerAPI::API::updateNotificationToken;

        Windows::Data::Json::JsonObject data;

        data.SetNamedValue(AppServerAPI::Key::appType, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::AppType));
        data.SetNamedValue(AppServerAPI::Key::appVer, Windows::Data::Json::JsonValue::CreateStringValue(appVersion));
        data.SetNamedValue(AppServerAPI::Key::notificationType, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::Key::longPolling));
        data.SetNamedValue(AppServerAPI::Key::notificationToken, Windows::Data::Json::JsonValue::CreateStringValue(AppServerAPI::Key::longPolling));

        Windows::Web::Http::HttpStatusCode statusCode;
        hstring response;

        post(uri, accessToken, data.ToString(), response, statusCode);

        if (false == isSuccessStatusCode(statusCode)) {
            return false;
        }

        try {
            auto responseResult = Windows::Data::Json::JsonObject::Parse(response);

            if (responseResult.TryLookup(AppServerAPI::Key::status)) {
                if (responseResult.Lookup(AppServerAPI::Key::status).GetString() == AppServerAPI::Key::success) {
                    return true;
                }
                else {
                    // do nothing
                }
            }
        }
        catch (winrt::hresult_error const&) {

        }

        return false;
    }

}
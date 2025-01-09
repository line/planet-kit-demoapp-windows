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

#include "SettingService.h"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Data.Json.h>

#if __has_include("SettingService.g.cpp")
#include "SettingService.g.cpp"
#endif

#include "LinePlanetCallVersion.h"
#include "PlanetKitManager.h"
#include "JwtDecoder.h"

namespace winrt::LINEPlanetCall::implementation {
    SettingService::SettingService() {
        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();

        std::wstring planetKitVersion = planetKitManager->PlanetKitVersion().c_str();

        auto pos = planetKitVersion.rfind(L'.');
        planetKitVersion.erase(pos, planetKitVersion.size() - pos);
        m_planetKitVersion = planetKitVersion;

        m_appVersion = AppVersion;
    }

    event_token SettingService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void SettingService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring SettingService::name() {
        return m_name;
    }

    void SettingService::name(hstring const& name) {
        if (m_name != name) {
            m_name = name;
            dispatchPropertyChanged(L"name");
        }
    }

    hstring SettingService::userId() {
        return m_userId;
    }

    void SettingService::userId(hstring const& userId) {
        if (m_userId != userId) {
            m_userId = userId;
            dispatchPropertyChanged(L"userId");
        }
    }

    hstring SettingService::serviceId() {
        return m_serviceId;
    }

    void SettingService::serviceId(hstring const& serviceId) {
        if (m_serviceId != serviceId) {
            m_serviceId = serviceId;
            dispatchPropertyChanged(L"serviceId");
        }
    }

    hstring SettingService::accessToken() {
        return m_accessToken;
    }

    AccountStatus SettingService::accountStatus() {
        return m_accountStatus;
    }

    void SettingService::accountStatus(AccountStatus status) {
        if (m_accountStatus != status) {
            m_accountStatus = status;

            if (m_accountStatus == AccountStatus::registered) {
                saveUserInfo();
            }

            dispatchPropertyChanged(L"accountStatus");
        }
    }

    int64_t SettingService::expiredTime() {
        return m_expiredTime;
    }

    hstring SettingService::planetKitVersion() {
        return m_planetKitVersion;
    }

    hstring SettingService::appVersion() {
        return m_appVersion;
    }

    Windows::Foundation::IAsyncOperation<AccountStatus> SettingService::registerUser() {
        auto status = AccountStatus::unregistered;
        if (m_userId.empty() || m_name.empty()) {
            accountStatus(status);
            co_return status;
        }

        bool skipRegisterUser = false;
        if (m_accessToken.empty() == false) {
            JwtDecoder jwtDecoder(m_accessToken.c_str());
            getExpiredTimeFromPayload(jwtDecoder.payload().c_str());

            // try to check expired time
            time_t currentTime = time(nullptr);
            skipRegisterUser = currentTime < m_expiredTime;

            if (skipRegisterUser == false) {
                // user id is expired.
                // force reset.
                status = AccountStatus::unregistered;

                resetUser();

                co_return status;
            }
        }
        
        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background(); // Return control; resume on thread pool.

        hstring accessToken;
        LINEPlanetCall::AppServerController appServerController;
        RegisterUserResult registerResult;

        if (skipRegisterUser == false) {
            registerResult = appServerController.registerUser(m_userId).get();

            status = registerResult.accountStatus;
            accessToken = registerResult.accessToken;
            m_accessToken = accessToken;

            // keep expired time
            JwtDecoder jwtDecoder(m_accessToken.c_str());
            getExpiredTimeFromPayload(jwtDecoder.payload().c_str());
        }
        else {
            accessToken = m_accessToken;
        }

        if (skipRegisterUser == true || status == AccountStatus::registered) {
            // try to register device
            bool registerDeviceResult = appServerController.registerDevice(accessToken, m_appVersion).get();

            if (registerDeviceResult == true) {
                status = AccountStatus::registered;
            }
            else {
                status = AccountStatus::unregistered;
            }
        }

        co_await ui_thread; // await ui thread

        // update property
        accountStatus(status);
        dispatchPropertyChanged(L"expiredTime");

        co_return status;
    }

    Windows::Foundation::IAsyncAction SettingService::loadUserInfo() {
        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background(); // Return control; resume on thread pool.

        Windows::Storage::StorageFolder storageFolder{ Windows::Storage::ApplicationData::Current().LocalFolder() };
        auto file{ co_await storageFolder.CreateFileAsync(SettingService::userInfoFile, Windows::Storage::CreationCollisionOption::OpenIfExists) };
        auto jsonString{ co_await Windows::Storage::FileIO::ReadTextAsync(file) };

        co_await ui_thread; // await ui thread

        if (jsonString.empty()) {
            // do nothing
            accountStatus(AccountStatus::unregistered);
        }
        else {
            Windows::Data::Json::JsonObject dataJson;
            Windows::Data::Json::JsonObject::TryParse(jsonString, dataJson);

            LINEPlanetCall::UserInfo userInfo(dataJson);

            name(userInfo.displayName());
            userId(userInfo.userId());
            m_accessToken = userInfo.accessToken();

            registerUser();
        }
    }

    void SettingService::resetUser() {
        m_name.clear();
        m_userId.clear();

        m_accessToken.clear();
        m_expiredTime = 0;

        dispatchPropertyChanged(L"name");
        dispatchPropertyChanged(L"userId");
        dispatchPropertyChanged(L"expiredTime");
        
        accountStatus(AccountStatus::unregistered);

        saveUserInfo();
    }

    Windows::Foundation::IAsyncOperation<NotificationResult> SettingService::notification() {
        NotificationResult result;
        result.statusCode = Windows::Web::Http::HttpStatusCode::BadRequest;

        if (m_accountStatus != AccountStatus::registered) {
            co_return result;
        }

        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background(); // Return control; resume on thread pool.

        result = m_appServerController.notification(m_accessToken, m_userId).get();

        co_await ui_thread; // await ui thread

        co_return result;
    }

    void SettingService::stopNotification() {
        m_appServerController.stopNotification();
    }

    void SettingService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    Windows::Foundation::IAsyncAction SettingService::saveUserInfo() {
        winrt::apartment_context ui_thread; // Capture calling context.
        co_await winrt::resume_background(); // Return control; resume on thread pool.

        Windows::Storage::StorageFolder storageFolder{ Windows::Storage::ApplicationData::Current().LocalFolder() };
        auto file{ co_await storageFolder.CreateFileAsync(SettingService::userInfoFile, Windows::Storage::CreationCollisionOption::OpenIfExists) };

        LINEPlanetCall::UserInfo userInfo(m_name, m_userId, m_accessToken);
        co_await Windows::Storage::FileIO::WriteTextAsync(file, userInfo.toString());
        
        co_await ui_thread; // await ui thread
    }

    void SettingService::getExpiredTimeFromPayload(hstring const& payload) {
        Windows::Data::Json::JsonObject jsonPayload;
        Windows::Data::Json::JsonObject::TryParse(payload, jsonPayload);

        if (jsonPayload.TryLookup(L"exp")) {
            m_expiredTime = (int64_t)jsonPayload.Lookup(L"exp").GetNumber();
        }
    }
}
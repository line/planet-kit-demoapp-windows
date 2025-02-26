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

#include "SettingService.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct SettingService : SettingServiceT<SettingService> {
        SettingService();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring name();
        void name(hstring const& name);

        hstring userId();
        void userId(hstring const& userId);

        hstring serviceId();
        void serviceId(hstring const& serviceId);

        hstring accessToken();

        AccountStatus accountStatus();
        void accountStatus(AccountStatus status);

        int64_t expiredTime();

        hstring planetKitVersion();
        hstring appVersion();

        Windows::Foundation::IAsyncOperation<AccountStatus> registerUser();
        Windows::Foundation::IAsyncAction loadUserInfo();

        void resetUser();

        Windows::Foundation::IAsyncOperation<NotificationResult> notification();
        void stopNotification();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

    private:
        Windows::Foundation::IAsyncAction saveUserInfo();

        void getExpiredTimeFromPayload(hstring const& payload);

    private:
        static constexpr inline wchar_t userInfoFile[] = L"userInfo.json";

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        hstring m_name;
        hstring m_userId;
        hstring m_serviceId;

        hstring m_accessToken;
        int64_t m_expiredTime;

        AccountStatus m_accountStatus{ AccountStatus::idle };

        hstring m_planetKitVersion;
        hstring m_appVersion;

        LINEPlanetCall::AppServerController m_appServerController;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct SettingService : SettingServiceT<SettingService, implementation::SettingService> {

    };
}
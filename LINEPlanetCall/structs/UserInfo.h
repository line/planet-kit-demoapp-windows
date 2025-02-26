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

#include "UserInfo.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct UserInfo : UserInfoT<UserInfo> {
        UserInfo(hstring const& displayName, hstring const& userId, hstring const& accessToken);
        UserInfo(Windows::Data::Json::JsonObject userInfo);

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring displayName();
        hstring userId();
        hstring accessToken();

        hstring toString();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

    private:
        static constexpr inline wchar_t userInfoKey[] = L"userInfo";
        static constexpr inline wchar_t dispalyNameKey[] = L"displayName";
        static constexpr inline wchar_t userIdKey[] = L"userId";
        static constexpr inline wchar_t accessTokenKey[] = L"accessToken";

    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        hstring m_displayName;
        hstring m_userId;
        hstring m_accessToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct UserInfo : UserInfoT<UserInfo, implementation::UserInfo> {

    };
}
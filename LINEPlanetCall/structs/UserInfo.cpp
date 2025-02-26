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

#include <winrt/Windows.Data.Json.h>

#include "UserInfo.h"

#if __has_include("UserInfo.g.cpp")
#include "UserInfo.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    UserInfo::UserInfo(hstring const& displayName, hstring const& userId, hstring const& accessToken) {
        m_displayName = displayName;
        m_userId = userId;
        m_accessToken = accessToken;
    }

    UserInfo::UserInfo(Windows::Data::Json::JsonObject userInfo) {
        auto userInfoData = userInfo.TryLookup(UserInfo::userInfoKey);

        if (userInfoData) {
            auto displayName = userInfoData.GetObjectW().TryLookup(dispalyNameKey);
            if (displayName) {
                m_displayName = displayName.GetString();
            }

            auto userId = userInfoData.GetObjectW().TryLookup(userIdKey);
            if (userId) {
                m_userId = userId.GetString();
            }

            auto accessToken = userInfoData.GetObjectW().TryLookup(accessTokenKey);
            if (accessToken) {
                m_accessToken = accessToken.GetString();
            }
        }
    }

    event_token UserInfo::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void UserInfo::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring UserInfo::displayName() {
        return m_displayName;
    }

    hstring UserInfo::userId() {
        return m_userId;
    }

    hstring UserInfo::accessToken() {
        return m_accessToken;
    }

    hstring UserInfo::toString() {
        Windows::Data::Json::JsonObject data;
        Windows::Data::Json::JsonObject userInfo;

        if (m_displayName.empty() == false) {
            data.SetNamedValue(UserInfo::dispalyNameKey, Windows::Data::Json::JsonValue::CreateStringValue(m_displayName));
        }

        if (m_userId.empty() == false) {
            data.SetNamedValue(UserInfo::userIdKey, Windows::Data::Json::JsonValue::CreateStringValue(m_userId));
        }

        if (m_accessToken.empty() == false) {
            data.SetNamedValue(UserInfo::accessTokenKey, Windows::Data::Json::JsonValue::CreateStringValue(m_accessToken));
        }
        
        userInfo.SetNamedValue(UserInfo::userInfoKey, data);

        return userInfo.ToString();
    }
    
    void UserInfo::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
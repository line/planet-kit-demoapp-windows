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

namespace AppServerAPI {
    constexpr wchar_t AppType[] = L"DESKTOPWIN";

    namespace API {
        constexpr wchar_t registerUser[] = L"/v2/register_user";
        constexpr wchar_t registerDevice[] = L"/v2/register_device";
        constexpr wchar_t updateNotificationToken[] = L"/v2/update_notification_token";
        constexpr wchar_t notification[] = L"/v2/notification/lp";
        constexpr wchar_t AccessToken[] = L"/v1/access_token/issue";

        constexpr wchar_t accessToken[] = L"/v2/access_token/issue";

        constexpr wchar_t regionKey[] = L"JP";
        constexpr wchar_t registerUserApiKey[] = L"e-Lx-xZxLXHpy0MlVudyjRAXJp1FOWN82eXIYyGyC7gmJh83U4IFQeTiaiKhvWxT5AVsuxVHztAdNUqQkXtGC0VsV2QgkQ-OuWyP57OChs-Ov_37NuTwS6sOD1Eb4PK5xQkiKoOd9nL2lqFBKqaxxg";
    };

    namespace Key {
        constexpr wchar_t UserId[] = L"user_id=";
        constexpr wchar_t ServiceId[] = L"service_id=";
        constexpr wchar_t Region[] = L"region=";
        constexpr wchar_t AppType[] = L"app_type=";
        constexpr wchar_t AppVersion[] = L"app_ver=";

        constexpr wchar_t userId[] = L"userId";
        constexpr wchar_t serviceId[] = L"serviceId";
        constexpr wchar_t region[] = L"region";
        constexpr wchar_t apiKey[] = L"apiKey";

        constexpr wchar_t appType[] = L"appType";
        constexpr wchar_t appVer[] = L"appVer";
        constexpr wchar_t notificationType[] = L"notificationType";
        constexpr wchar_t notificationToken[] = L"notificationToken";

        constexpr wchar_t data[] = L"data";
        constexpr wchar_t accessToken[] = L"accessToken";
        constexpr wchar_t status[] = L"status";
        constexpr wchar_t gwAccessToken[] = L"gwAccessToken";

        constexpr wchar_t longPolling[] = L"lp";

        constexpr wchar_t success[] = L"success";

        constexpr wchar_t callerUserId[] = L"app_caller_uid";
        constexpr wchar_t ccParam[] = L"cc_param";
        constexpr wchar_t appCallType[] = L"app_call_type";
    }
};
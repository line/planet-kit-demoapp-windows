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
#include "AlertMessagePage.xaml.h"
#if __has_include("AlertMessagePage.g.cpp")
#include "AlertMessagePage.g.cpp"
#endif

#include "PlanetKitManager.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t AlertMessagePage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void AlertMessagePage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    hstring AlertMessagePage::title() {
        return m_title;
    }

    hstring AlertMessagePage::subTitle() {
        return m_subTitle;
    }

    hstring AlertMessagePage::reason() {
        return m_reason;
    }

    hstring AlertMessagePage::buttonText() {
        return m_button;
    }

    void AlertMessagePage::disconnectReason(int32_t reason) {
        m_disconnectReason = reason;
        m_reason = convert(m_disconnectReason);

        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
        m_title = resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall1/text");
        m_subTitle = resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall2/text");
        m_button = resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall3/text");
    }

    int32_t AlertMessagePage::disconnectReason() {
        return m_disconnectReason;
    }

    void AlertMessagePage::appFailedReason(AppFailedReason appFailedReason) {
        m_appFailedReason = appFailedReason;
        m_reason = convertAppFailedReason(appFailedReason);

        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
        m_title = resourceLoader.GetString(L"lp_demoapp_common_error_startfail0/text");
        m_subTitle = resourceLoader.GetString(L"lp_demoapp_common_default_error_msg/text");
        m_button = resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall3/text");
    }

    AppFailedReason AlertMessagePage::appFailedReason() {
        return m_appFailedReason;
    }

    void AlertMessagePage::planetKitFailedReason(int32_t reason) {
        m_planetKitFailedReason = reason;
        m_reason = convertPlanetKitFailedReason(m_planetKitFailedReason);

        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
        m_title = resourceLoader.GetString(L"lp_demoapp_common_error_startfail0/text");
        m_subTitle = resourceLoader.GetString(L"lp_demoapp_common_default_error_msg/text");
        m_button = resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall3/text");
    }

    int32_t AlertMessagePage::planetKitFailedReason() {
        return m_planetKitFailedReason;
    }

    Microsoft::UI::Xaml::Controls::ContentDialog AlertMessagePage::dialog() {
        return m_dialog;
    }

    void AlertMessagePage::dialog(Microsoft::UI::Xaml::Controls::ContentDialog const& dialog) {
        m_dialog = dialog;
    }

    void AlertMessagePage::clickClose(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_dialog.Hide();
    }

    hstring AlertMessagePage::convert(int32_t disconnectReason) {
        hstring reason;

        switch (disconnectReason) {
        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_NORMAL :
            reason = L"NORMAL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_DECLINE :
            reason = L"DECLINE";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_CELL_CALL:
            reason = L"CELL_CALL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_INTERNAL_ERROR:
            reason = L"INTERNAL_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_USER_ERROR:
            reason = L"USER_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_INTERNAL_KIT_ERROR:
            reason = L"INTERNAL_KIT_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_AUDIO_TX_NO_SRC:
            reason = L"TX_NO_SRC";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_CANCEL:
            reason = L"CANCEL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_BUSY:
            reason = L"BUSY";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_NOANSWER:
            reason = L"NOANSWER";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_ALREADY_GOT_A_CALL:
            reason = L"GOT_A_CALL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_MULTIDEV_IN_USE:
            reason = L"MULTIDEV_IN_USE";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_MULTIDEV_ANSWER:
            reason = L"MULTIDEV_ANSWER";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_MULTIDEV_DECLINE:
            reason = L"MULTIDEV_DECLINE";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_MAX_CALL_TIME_EXCEEDED:
            reason = L"MAX_CALL_TIME_EXCEEDED";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_NETWORK_UNSTABLE:
            reason = L"NETWORK_UNSTABLE";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_PUSH_ERROR:
            reason = L"PUSH_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_AUTH_ERROR:
            reason = L"AUTH_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_RELEASED_CALL:
            reason = L"RELEASED_CALL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVER_INTERNAL_ERROR:
            reason = L"SERVER_INTERNAL_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_UNAVAILABLE_NETWORK:
            reason = L"UNAVAILABLE_NETWORK";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_APP_DESTROY:
            reason = L"APP_DESTROY";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SYSTEM_SLEEP:
            reason = L"SYSTEM_SLEEP";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SYSTEM_LOGOFF:
            reason = L"SYSTEM_LOGOFF";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_MTU_EXCEEDED:
            reason = L"MTU_EXCEEDED";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_APP_SERVER_DATA_ERROR:
            reason = L"APP_SERVER_DATA_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_ROOM_IS_FULL:
            reason = L"ROOM_IS_FULL";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_ALONE_KICK_OUT:
            reason = L"ALONE_KICK_OUT";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_ROOM_NOT_FOUND:
            reason = L"ROOM_NOT_FOUND";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_ANOTHER_INSTANCE_TRY_TO_JOIN:
            reason = L"INSTANCE_TRY_TO_JOIN";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_ACCESS_TOKEN_ERROR:
            reason = L"SERVICE_ACCESS_TOKEN_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_INVALID_ID:
            reason = L"SERVICE_INVALID_ID";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_MAINTENANCE:
            reason = L"SERVICE_MAINTENANCE";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_BUSY:
            reason = L"SERVICE_BUSY";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_INTERNAL_ERROR:
            reason = L"SERVICE_INTERNAL_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_HTTP_INVALID_PEER_CERT:
            reason = L"SERVICE_HTTP_INVALID_PEER_CERT";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_HTTP_ERROR:
            reason = L"SERVICE_HTTP_ERROR";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_HTTP_CONNECTION_TIME_OUT:
            reason = L"HTTP_CONNECTION_TIME_OUT";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_INCOMPATIBLE_PLANETKIT_VER:
            reason = L"SERVICE_INCOMPATIBLE_PLANETKIT_VER";
            break;

        case PlanetKit::EDisconnectReason::PLNK_DISCONNECT_REASON_SERVICE_TOO_MANY_REQUESTS:
            reason = L"SERVICE_TOO_MANY_REQUESTS";
            break;

        default:
            reason = L"INVALID_ERROR";
            break;
        }

        return reason;
    }

    hstring AlertMessagePage::convertAppFailedReason(AppFailedReason appFailedReason) {
        hstring reason;
        switch (appFailedReason) {
        case AppFailedReason::none:
            reason = L"NONE";
            break;

        case AppFailedReason::emptyId:
            reason = L"USER_ID_IS_EMPTY";
            break;

        case AppFailedReason::emptyPeerId:
            reason = L"PEER_ID_IS_EMPTY";
            break;

        case AppFailedReason::expiredAccessToken:
            reason = L"USER_ID_IS_EXPIRED";
            break;

        default:
            reason = L"INVALID_APP_ERROR";
            break;
        }

        return reason;
    }

    hstring AlertMessagePage::convertPlanetKitFailedReason(int32_t planetKitFailedReason) {
        hstring reason;
        switch (planetKitFailedReason) {
        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_NONE:
            reason = L"NONE";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INVALID_PARAM:
            reason = L"INVALID_PARAM";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_ALREADY_EXIST:
            reason = L"ALREADY_EXIST";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_DECODE_CALL_PARAM:
            reason = L"DECODE_CALL_PARAM";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_MEM_ERR:
            reason = L"MEM_ERR";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_ID_CONFLICT:
            reason = L"ID_CONFLICT";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_REUSE:
            reason = L"REUSE";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INVALID_USER_ID:
            reason = L"INVALID_USER_ID";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INVALID_SERVICE_ID:
            reason = L"INVALID_SERVICE_ID";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INVALID_API_KEY:
            reason = L"INVALID_API_KEY";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INVALID_ROOM_ID:
            reason = L"INVALID_ROOM_ID";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_TOO_LONG_APP_SERVER_DATA:
            reason = L"TOO_LONG_APP_SERVER_DATA";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_NOT_INITIALIZED:
            reason = L"NOT_INITIALIZED";
            break;

        case PlanetKit::EStartFailReason::PLNK_START_FAIL_REASON_INTERNAL:
            reason = L"INTERNAL";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_CORE_LIBRARY_FAILED:
            reason = L"CORE_LIBRARY_FAILED";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_AUDIO_INPUT_DEVICE_FAIL:
            reason = L"AUDIO_INPUT_DEVICE_FAIL";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_AUDIO_OUTPUT_DEVICE_FAIL:
            reason = L"AUDIO_OUTPUT_DEVICE_FAIL";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_VIDEO_CAPTURER_FAIL:
            reason = L"VIDEO_CAPTURER_FAIL";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_NOT_READY:
            reason = L"NOT_READY";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_INVALID_PARAM:
            reason = L"INVALID_PARAM";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_OTHER_CONFERENCE_IS_ALREADY_EXIST:
            reason = L"OTHER_CONFERENCE_IS_ALREADY_EXIST";
            break;

        case PlanetKit::EStartFailReason::PLNK_KIT_START_FAIL_REASON_PEER_ID_IS_EMPTY:
            reason = L"PEER_ID_IS_EMPTY";
            break;

        default:
            reason = L"INVALID_ERROR";
            break;
        }

        return reason;
    }
}

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

#include "SettingViewModel.h"

#if __has_include("SettingViewModel.g.cpp")
#include "SettingViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    SettingViewModel::SettingViewModel(LINEPlanetCall::SettingService settingService) {
        m_settingService = settingService;

        // Register model's property changed event
        m_settingServiceToken = m_settingService.PropertyChanged({ this, &SettingViewModel::settingServiceProperyChanged });
    }

    SettingViewModel::~SettingViewModel() {
        // deregister event
        m_settingService.PropertyChanged(m_settingServiceToken);
    }

    event_token SettingViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void SettingViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    hstring SettingViewModel::name() {
        return m_settingService.name();
    }

    void SettingViewModel::name(hstring const& name) {
        if (m_settingService.name() != name) {
            m_settingService.name(name);
            dispatchPropertyChanged(L"name");
        }
    }

    hstring SettingViewModel::userId() {
        return m_settingService.userId();
    }

    void SettingViewModel::userId(hstring const& userId) {
        if (m_settingService.userId() != userId) {
            m_settingService.userId(userId);
            dispatchPropertyChanged(L"userId");
        }
    }

    hstring SettingViewModel::serviceId() {
        return m_settingService.serviceId();
    }

    void SettingViewModel::serviceId(hstring const& serviceId) {
        if (m_settingService.serviceId() != serviceId) {
            m_settingService.serviceId(serviceId);
            dispatchPropertyChanged(L"serviceId");
        }
    }

    AccountStatus SettingViewModel::accountStatus() {
        return m_settingService.accountStatus();
    }

    void SettingViewModel::accountStatus(AccountStatus accountStatus) {
        if (m_settingService.accountStatus() != accountStatus) {
            m_settingService.accountStatus(accountStatus);
            dispatchPropertyChanged(L"accountStatus");
        }
    }

    hstring SettingViewModel::expiredGuide() {
        static std::wstring replaceStr = L"{{YYYY-MM-DD hh.mm.ss}} {{gmt info}}";

        int64_t expiredTime = m_settingService.expiredTime();
        hstring result;

        if (expiredTime != 0) {
            time_t t(expiredTime);
            struct tm timeInfo { 0 };
            localtime_s(&timeInfo, &t);

            wchar_t timeString[30] = { 0, };
            std::wcsftime(timeString, 30, L"%Y-%m-%d %H:%M:%S", &timeInfo);

            Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
            std::wstring message = resourceLoader.GetString(L"lp_demoapp_setting_guide4/text").c_str();

            message.replace(message.find(replaceStr), replaceStr.size(), timeString);

            result = message;
        }

        return result;
    }

    bool SettingViewModel::enableUserName() {
        auto accountStatus = m_settingService.accountStatus();

        if (accountStatus == AccountStatus::registered) {
            return false;
        }
        else {
            return true;
        }
    }

    bool SettingViewModel::enableUserId() {
        auto accountStatus = m_settingService.accountStatus();

        if (accountStatus == AccountStatus::registered) {
            return false;
        }
        else {
            return true;
        }
    }

    bool SettingViewModel::enableScenario() {
        auto accountStatus = m_settingService.accountStatus();

        if (accountStatus == AccountStatus::registered) {
            return true;
        }
        else {
            return false;
        }
    }

    Microsoft::UI::Xaml::Visibility SettingViewModel::visibleGuideDot() {
        auto accountStatus = m_settingService.accountStatus();

        Microsoft::UI::Xaml::Visibility visible = Microsoft::UI::Xaml::Visibility::Collapsed;

        if (accountStatus == AccountStatus::registered || accountStatus == AccountStatus::idle) {
            visible = Microsoft::UI::Xaml::Visibility::Collapsed;
        }
        else {
            visible = Microsoft::UI::Xaml::Visibility::Visible;
        }

        return visible;
    }

    Microsoft::UI::Xaml::Visibility SettingViewModel::visibleGuideMessage() {
        auto accountStatus = m_settingService.accountStatus();

        Microsoft::UI::Xaml::Visibility visible = Microsoft::UI::Xaml::Visibility::Collapsed;

        if (accountStatus == AccountStatus::registered || accountStatus == AccountStatus::idle) {
            visible = Microsoft::UI::Xaml::Visibility::Collapsed;
        }
        else {
            visible = Microsoft::UI::Xaml::Visibility::Visible;
        }

        return visible;
    }

    Microsoft::UI::Xaml::Visibility SettingViewModel::visibleRegister() {
        return m_settingService.accountStatus() != AccountStatus::registered ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }

    Microsoft::UI::Xaml::Visibility SettingViewModel::visibleReset() {
        return m_settingService.accountStatus() == AccountStatus::registered ? Microsoft::UI::Xaml::Visibility::Visible : Microsoft::UI::Xaml::Visibility::Collapsed;
    }

    Windows::Foundation::IAsyncOperation<AccountStatus> SettingViewModel::registerUser() {
        co_return co_await m_settingService.registerUser();
    }

    void SettingViewModel::resetUser() {
        m_settingService.resetUser();
    }

    void SettingViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void SettingViewModel::settingServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        dispatchPropertyChanged(e.PropertyName());

        if (e.PropertyName() == L"accountStatus") {
            dispatchPropertyChanged(L"visibleRegister");
            dispatchPropertyChanged(L"visibleReset");
            dispatchPropertyChanged(L"visibleGuideMessage");
            dispatchPropertyChanged(L"visibleGuideDot");
            dispatchPropertyChanged(L"enableScenario");
            dispatchPropertyChanged(L"enableUserName");
            dispatchPropertyChanged(L"enableUserId");            
        }
        else if (e.PropertyName() == L"expiredTime") {
            dispatchPropertyChanged(L"expiredGuide");
        }
    }
}
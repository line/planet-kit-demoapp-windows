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

#include "SettingViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct SettingViewModel : SettingViewModelT<SettingViewModel> {
        SettingViewModel(LINEPlanetCall::SettingService settingService);
        virtual ~SettingViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring name();
        void name(hstring const& name);

        hstring userId();
        void userId(hstring const& userId);

        hstring serviceId();
        void serviceId(hstring const& serviceId);

        AccountStatus accountStatus();
        void accountStatus(AccountStatus accountStatus);

        hstring expiredGuide();

        bool enableUserName();
        bool enableUserId();

        bool enableScenario();

        Microsoft::UI::Xaml::Visibility visibleGuideDot();

        Microsoft::UI::Xaml::Visibility visibleGuideMessage();

        Microsoft::UI::Xaml::Visibility visibleRegister();
        Microsoft::UI::Xaml::Visibility visibleReset();

        Windows::Foundation::IAsyncOperation<AccountStatus> registerUser();
        void resetUser();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        // Event listener for changing of model's property.
        void settingServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::SettingService m_settingService{ nullptr };
        event_token m_settingServiceToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct SettingViewModel : SettingViewModelT<SettingViewModel, implementation::SettingViewModel> {

    };
}
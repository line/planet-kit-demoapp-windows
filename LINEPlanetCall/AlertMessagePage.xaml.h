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

#include "AlertMessagePage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct AlertMessagePage : AlertMessagePageT<AlertMessagePage>
    {
        AlertMessagePage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        hstring title();
        hstring subTitle();
        hstring reason();
        hstring buttonText();

        void disconnectReason(int32_t reason);
        int32_t disconnectReason();

        void appFailedReason(AppFailedReason appFailedReason);
        AppFailedReason appFailedReason();

        void planetKitFailedReason(int32_t reason);
        int32_t planetKitFailedReason();

        Microsoft::UI::Xaml::Controls::ContentDialog dialog();
        void dialog(Microsoft::UI::Xaml::Controls::ContentDialog const& dialog);

        void clickClose(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private :
        hstring convert(int32_t disconnectReason);
        hstring convertAppFailedReason(AppFailedReason appFailedReason);
        hstring convertPlanetKitFailedReason(int32_t planetKitFailedReason);

    private :
        hstring m_title;
        hstring m_subTitle;
        hstring m_reason;
        hstring m_button;

        int32_t m_disconnectReason = 0;
        AppFailedReason m_appFailedReason = AppFailedReason::none;
        int32_t m_planetKitFailedReason = 0;

        Microsoft::UI::Xaml::Controls::ContentDialog m_dialog{ nullptr };
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct AlertMessagePage : AlertMessagePageT<AlertMessagePage, implementation::AlertMessagePage>
    {
    };
}

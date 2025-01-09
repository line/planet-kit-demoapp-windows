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

#include "AlertPage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct AlertPage : AlertPageT<AlertPage>
    {
        AlertPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void title(hstring const& title);
        hstring title();

        void content(hstring const& content);
        hstring content();

        void buttonString(hstring const& buttonString);
        hstring buttonString();

        Microsoft::UI::Xaml::Controls::ContentDialog dialog();
        void dialog(Microsoft::UI::Xaml::Controls::ContentDialog const& dialog);

        void clickClose(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        hstring m_title;
        hstring m_content;
        hstring m_buttonString;

        Microsoft::UI::Xaml::Controls::ContentDialog m_dialog{ nullptr };
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct AlertPage : AlertPageT<AlertPage, implementation::AlertPage>
    {
    };
}

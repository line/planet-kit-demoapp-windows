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

#include "MainPage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        LINEPlanetCall::MainWindow parentWindow();
        void parentWindow(LINEPlanetCall::MainWindow parent);

        LINEPlanetCall::SettingViewModel settingViewModel();

        void clickSettings(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickGroupCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private :
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };
        LINEPlanetCall::SettingViewModel m_settingViewModel{ nullptr };
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

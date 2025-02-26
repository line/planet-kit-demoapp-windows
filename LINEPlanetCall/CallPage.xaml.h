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

#include "CallPage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct CallPage : CallPageT<CallPage>
    {
        CallPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        LINEPlanetCall::MainWindow parentWindow();
        void parentWindow(LINEPlanetCall::MainWindow const& parent);

        LINEPlanetCall::CallViewModel callViewModel();

        void clickBack(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickHome(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void clickAudioCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickVideoCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void peerIdTextChanging(Microsoft::UI::Xaml::Controls::TextBox const& sender, Microsoft::UI::Xaml::Controls::TextBoxTextChangingEventArgs const& args);

    private:
        bool checkPeerId();
        Windows::Foundation::IAsyncAction showAlert(hstring title, hstring content, hstring buttonString);

    private:
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };
        LINEPlanetCall::CallViewModel m_callViewModel{ nullptr };
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct CallPage : CallPageT<CallPage, implementation::CallPage>
    {
    };
}

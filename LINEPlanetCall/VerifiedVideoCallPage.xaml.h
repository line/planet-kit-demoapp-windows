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

#include "VerifiedVideoCallPage.g.h"

#include "VideoRender.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct VerifiedVideoCallPage : VerifiedVideoCallPageT<VerifiedVideoCallPage>
    {
        VerifiedVideoCallPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        virtual ~VerifiedVideoCallPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();

        LINEPlanetCall::MainWindow parentWindow();
        void parentWindow(LINEPlanetCall::MainWindow parentWindow);

        LINEPlanetCall::VerifiedCallViewModel verifiedCallViewModel();

        void clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickVideoEnable(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args);

    protected:
        void verifiedCallViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };

        LINEPlanetCall::VerifiedCallViewModel m_verifiedCallViewModel{ nullptr };
        event_token m_verifiedCallViewModelEventToken;

        VideoRender m_videoRender;
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct VerifiedVideoCallPage : VerifiedVideoCallPageT<VerifiedVideoCallPage, implementation::VerifiedVideoCallPage>
    {
    };
}
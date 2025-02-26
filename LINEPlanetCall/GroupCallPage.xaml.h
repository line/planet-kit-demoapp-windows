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

#include "GroupCallPage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct GroupCallPage : GroupCallPageT<GroupCallPage>
    {
        GroupCallPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        virtual ~GroupCallPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        LINEPlanetCall::MainWindow parentWindow();
        void parentWindow(LINEPlanetCall::MainWindow parent);

        LINEPlanetCall::GroupCallViewModel groupCallViewModel();
        LINEPlanetCall::GroupCallParticipantListViewModel groupCallParticipantListViewModel();

        void leaveConference();

        void clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickVideoEnable(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void clickLeave(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void groupCallPageSizeChanged(IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& args);

    protected:
        void groupCallViewModelPropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };

        LINEPlanetCall::GroupCallViewModel m_groupCallViewModel{ nullptr };
        event_token m_groupCallViewModelToken;

        LINEPlanetCall::GroupCallParticipantListViewModel m_groupCallParticipantListViewModel{ nullptr };
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct GroupCallPage : GroupCallPageT<GroupCallPage, implementation::GroupCallPage>
    {
    };
}

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

#include "GroupCallParticipantPage.g.h"
#include "VideoRender.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct GroupCallParticipantPage : GroupCallParticipantPageT<GroupCallParticipantPage>
    {
        GroupCallParticipantPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        virtual ~GroupCallParticipantPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();

        LINEPlanetCall::GroupCallParticipantService groupCallParticipantService();
        void groupCallParticipantService(LINEPlanetCall::GroupCallParticipantService groupCallParticipantService);

        LINEPlanetCall::GroupCallParticipantViewModel groupCallParticipantViewModel();

        void sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args);

    protected:
        void groupCallParticipantViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        LINEPlanetCall::GroupCallParticipantService m_groupCallParticipantService{ nullptr };
        LINEPlanetCall::GroupCallParticipantViewModel m_groupCallParticipantViewModel{ nullptr };

        event_token m_groupCallParticipantViewModelEventToken;

        VideoRender m_videoRender;
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct GroupCallParticipantPage : GroupCallParticipantPageT<GroupCallParticipantPage, implementation::GroupCallParticipantPage>
    {
    };
}

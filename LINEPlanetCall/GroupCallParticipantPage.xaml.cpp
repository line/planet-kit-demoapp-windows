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
#include "GroupCallParticipantPage.xaml.h"
#if __has_include("GroupCallParticipantPage.g.cpp")
#include "GroupCallParticipantPage.g.cpp"
#endif

#include "GroupCallParticipantViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    GroupCallParticipantPage::~GroupCallParticipantPage() {
        if (m_groupCallParticipantViewModel) {
            m_groupCallParticipantViewModel.PropertyChanged(m_groupCallParticipantViewModelEventToken);
        }
    }

    int32_t GroupCallParticipantPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void GroupCallParticipantPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void GroupCallParticipantPage::InitializeComponent() {
        GroupCallParticipantPageT::InitializeComponent();

        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel swapChainPanel = renderPanel();
        m_videoRender.setSwapChainPanel(swapChainPanel);
    }

    LINEPlanetCall::GroupCallParticipantService GroupCallParticipantPage::groupCallParticipantService() {
        return m_groupCallParticipantService;
    }

    void GroupCallParticipantPage::groupCallParticipantService(LINEPlanetCall::GroupCallParticipantService groupCallParticipantService) {
        m_groupCallParticipantService = groupCallParticipantService;

        if (m_groupCallParticipantViewModel) {
            m_groupCallParticipantViewModel.updateService(m_groupCallParticipantService);
            m_groupCallParticipantViewModel.as<GroupCallParticipantViewModel>()->registerVideoRender(&m_videoRender);
        }
        else {
            m_groupCallParticipantViewModel = make<GroupCallParticipantViewModel>(m_groupCallParticipantService);
            m_groupCallParticipantViewModelEventToken = m_groupCallParticipantViewModel.PropertyChanged({ this, &GroupCallParticipantPage::groupCallParticipantViewModelProperyChanged });

            m_groupCallParticipantViewModel.as<GroupCallParticipantViewModel>()->registerVideoRender(&m_videoRender);
        }
    }

    LINEPlanetCall::GroupCallParticipantViewModel GroupCallParticipantPage::groupCallParticipantViewModel() {
        return m_groupCallParticipantViewModel;
    }

    void GroupCallParticipantPage::sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        float_t width = (float_t)renderPanel().ActualWidth();
        float_t height = (float_t)renderPanel().ActualHeight();

        m_videoRender.updateSize(width, height);

        // if video is not played.
        if (m_groupCallParticipantViewModel.videoEnabled() == false) {
            m_videoRender.drawBackground();
        }
    }

    void GroupCallParticipantPage::groupCallParticipantViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"videoEnabled") {
            bool paused = !m_groupCallParticipantViewModel.videoEnabled();
            m_videoRender.paused(paused);

            if (paused) {
                m_videoRender.drawBackground();
            }
        }
    }
}

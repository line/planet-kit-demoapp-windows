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
#include "GroupCallPage.xaml.h"
#if __has_include("GroupCallPage.g.cpp")
#include "GroupCallPage.g.cpp"
#endif

#include "UiUtils.h"
#include "GroupCallViewModel.h"
#include "GroupCallParticipantListViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    GroupCallPage::~GroupCallPage() {
        m_groupCallViewModel.PropertyChanged(m_groupCallViewModelToken);
        m_groupCallParticipantListViewModel.clear();
    }

    int32_t GroupCallPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void GroupCallPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow GroupCallPage::parentWindow() {
        return m_parentWindow;
    }

    void GroupCallPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        m_groupCallViewModel = make<GroupCallViewModel>(m_parentWindow.groupCallService());
        m_groupCallViewModelToken = m_groupCallViewModel.PropertyChanged({ this, &GroupCallPage::groupCallViewModelPropertyChanged });

        m_groupCallParticipantListViewModel = make<GroupCallParticipantListViewModel>(m_parentWindow);
    }

    LINEPlanetCall::GroupCallViewModel GroupCallPage::groupCallViewModel() {
        return m_groupCallViewModel;
    }

    LINEPlanetCall::GroupCallParticipantListViewModel GroupCallPage::groupCallParticipantListViewModel() {
        return m_groupCallParticipantListViewModel;
    }

    void GroupCallPage::leaveConference() {
        m_groupCallViewModel.leaveConference();
    }

    void GroupCallPage::clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_groupCallViewModel.muteMyAudio();
    }

    void GroupCallPage::clickVideoEnable(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (m_groupCallViewModel.myVideoEnabled() == true) {
            m_groupCallViewModel.pauseMyVideo();
        }
        else {
            m_groupCallViewModel.resumeMyVideo();
        }
    }

    void GroupCallPage::clickLeave(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_groupCallViewModel.leaveConference();
    }

    void GroupCallPage::groupCallPageSizeChanged(IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        static constexpr int32_t columeCount = 3;
        static constexpr int32_t rowCount = 2;
        static constexpr double_t upperPadding = 74.0f;
        static constexpr double_t lowerPadding = 74.0f;

        static constexpr double_t widthPadding = 5.0f;
        static constexpr double_t heightPadding = 5.0f;

        static constexpr double_t columeSpacing = 5.0f;
        static constexpr double_t rowSpacing = 5.0f;

        static constexpr double_t swapChainPanelPadding = 5.0f;

        double_t width = ActualWidth() - (swapChainPanelPadding * 2);
        double_t height = ActualHeight() - (swapChainPanelPadding * 2);

        double_t itemWidth = max((width - widthPadding * 2 - columeSpacing * 2) / columeCount, Constants::minParticipantWidth);
        double_t itemHeight = max((height - heightPadding * 2 - upperPadding - lowerPadding - rowSpacing) / rowCount, Constants::minParticipantHeight);

        m_groupCallParticipantListViewModel.resizeItem(itemWidth, itemHeight);

        itemsGrid().InvalidateMeasure();
    }

    void GroupCallPage::groupCallViewModelPropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"groupCallState") {
            if (m_groupCallViewModel.groupCallState() == ConnectState::disconnected) {
                m_parentWindow.endCall(m_groupCallViewModel.disconnectReason());
            }
        }
    }
}

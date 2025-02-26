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
#include "GroupCallScenarioPage.xaml.h"
#if __has_include("GroupCallScenarioPage.g.cpp")
#include "GroupCallScenarioPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t GroupCallScenarioPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void GroupCallScenarioPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow GroupCallScenarioPage::parentWindow() {
        return m_parentWindow;
    }

    void GroupCallScenarioPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;
    }

    void GroupCallScenarioPage::clickBack(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        navigateMainPage();
    }

    void GroupCallScenarioPage::clickHome(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        navigateMainPage();
    }

    void GroupCallScenarioPage::clickGroupCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto typeCallRoomNamePage = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallRoomNamePage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeCallRoomNamePage, nullptr, slide);
        mainFrame.Content().as<GroupCallRoomNamePage>().parentWindow(m_parentWindow);
    }

    void GroupCallScenarioPage::navigateMainPage() {
        auto typeMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeMainPage, nullptr, slide);
        mainFrame.Content().as<MainPage>().parentWindow(m_parentWindow);
    }
}

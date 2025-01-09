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
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include "MainWindow.xaml.h"
#include "SettingViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow MainPage::parentWindow() {
        return m_parentWindow;
    }

    void MainPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        m_settingViewModel = make<SettingViewModel>(m_parentWindow.settingService());
    }

    LINEPlanetCall::SettingViewModel MainPage::settingViewModel() {
        return m_settingViewModel;
    }

    void MainPage::clickSettings(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        // Navigate settings page.
        auto typeSettingsPage = winrt::xaml_typename<winrt::LINEPlanetCall::SettingsPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);
        
        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeSettingsPage, nullptr, slide);
        mainFrame.Content().as<SettingsPage>().parentWindow(m_parentWindow);
    }

    void MainPage::clickCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        // Navigate peer to peer call page.
        auto typeCallScenarioPage = winrt::xaml_typename<winrt::LINEPlanetCall::CallScenarioPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeCallScenarioPage, nullptr, slide);
        mainFrame.Content().as<CallScenarioPage>().parentWindow(m_parentWindow);
    }

    void MainPage::clickGroupCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        // Navigate group call page.
        auto typeGroupCallScenarioPage = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallScenarioPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeGroupCallScenarioPage, nullptr, slide);
        mainFrame.Content().as<GroupCallScenarioPage>().parentWindow(m_parentWindow);
    }
}

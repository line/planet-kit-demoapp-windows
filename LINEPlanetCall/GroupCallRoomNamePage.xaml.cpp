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
#include "GroupCallRoomNamePage.xaml.h"
#if __has_include("GroupCallRoomNamePage.g.cpp")
#include "GroupCallRoomNamePage.g.cpp"
#endif

#include "UiUtils.h"
#include "GroupCallRoomNameViewModel.h"

#include "UiUtils.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t GroupCallRoomNamePage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void GroupCallRoomNamePage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    LINEPlanetCall::MainWindow GroupCallRoomNamePage::parentWindow() {
        return m_parentWindow;
    }

    void GroupCallRoomNamePage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        m_groupCallRoomNameViewModel = make<GroupCallRoomNameViewModel>(m_parentWindow.groupCallService());
    }

    LINEPlanetCall::GroupCallRoomNameViewModel GroupCallRoomNamePage::groupCallRoomNameViewModel() {
        return m_groupCallRoomNameViewModel;
    }

    void GroupCallRoomNamePage::clickBack(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto typeGroupCallScenarioPage = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallScenarioPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeGroupCallScenarioPage, nullptr, slide);
        mainFrame.Content().as<GroupCallScenarioPage>().parentWindow(m_parentWindow);
    }

    void GroupCallRoomNamePage::clickHome(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto typeMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeMainPage, nullptr, slide);
        mainFrame.Content().as<MainPage>().parentWindow(m_parentWindow);
    }

    void GroupCallRoomNamePage::clickEnterPreview(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (m_groupCallRoomNameViewModel.roomId().empty() == true) {
            // enter room id
            showAlert();
            return;
        }

        auto typeGroupCallCameraPreview = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallCameraPreviewPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromBottom);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeGroupCallCameraPreview, nullptr, slide);
        mainFrame.Content().as<GroupCallCameraPreviewPage>().parentWindow(m_parentWindow);

        UiUtils::resizeWindow(m_parentWindow.AppWindow(), { 602, 447 });
    }

    void GroupCallRoomNamePage::roomIdTextChanging(Microsoft::UI::Xaml::Controls::TextBox const& sender, Microsoft::UI::Xaml::Controls::TextBoxTextChangingEventArgs const& args) {
        std::ignore = args;

        if (sender.Text().empty()) {
            return;
        }

        wchar_t value = *sender.Text().rbegin();

        if (UiUtils::isAvailableChar(value) == false) {
            std::wstring removed = sender.Text().c_str();
            removed.erase(removed.size() - 1);
            textBoxRoomId().Text(removed.c_str());

            textBoxRoomId().Select(textBoxRoomId().Text().size(), 0);
        }
    }

    Windows::Foundation::IAsyncAction GroupCallRoomNamePage::showAlert() {
        LINEPlanetCall::AlertPage alertPage;
        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

        alertPage.title(resourceLoader.GetString(L"lp_demoapp_common_error_startfail0/text"));
        alertPage.content(resourceLoader.GetString(L"lp_demoapp_common_error_startfail2/text"));
        alertPage.buttonString(resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall3/text"));

        Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(XamlRoot());
        dialog.Content(alertPage);
        alertPage.dialog(dialog);

        co_await dialog.ShowAsync();
    }
}

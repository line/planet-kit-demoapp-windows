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
#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif

#include "SettingViewModel.h"
#include "UiUtils.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t SettingsPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SettingsPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow SettingsPage::parentWindow() {
        return m_parentWindow;
    }

    void SettingsPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        m_settingViewModel = make<SettingViewModel>(m_parentWindow.settingService());
    }

    LINEPlanetCall::SettingViewModel SettingsPage::settingViewModel() {
        return m_settingViewModel;
    }

    void SettingsPage::clickBack(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        navigateMainPage();
    }

    void SettingsPage::clickCancel(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        navigateMainPage();
    }

    void SettingsPage::clickSave(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (checkMadatory() == false) {
            // show alert
            Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

            this->showError(
                resourceLoader.GetString(L"lp_demoapp_setting_popup1/Text"),
                resourceLoader.GetString(L"lp_demoapp_setting_error_savefail/Text"),
                resourceLoader.GetString(L"lp_demoapp_setting_popup3/Text")
            );

            return;
        }

        // register user id.
        auto registerOperation = m_settingViewModel.registerUser();

        // wait for operation completed.
        registerOperation.Completed([this](Windows::Foundation::IAsyncOperation<AccountStatus> const& result, auto const& status) {
            if (status == Windows::Foundation::AsyncStatus::Completed) {
                if (result.GetResults() == AccountStatus::registered) {
                    // do nothing
                }
                else if (result.GetResults() == AccountStatus::conflict) {
                    // failed to register user
                    Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

                    this->showError(
                        resourceLoader.GetString(L"lp_demoapp_setting_popup1/Text"),
                        resourceLoader.GetString(L"lp_demoapp_setting_popup2/Text"),
                        resourceLoader.GetString(L"lp_demoapp_setting_popup3/Text")
                    );
                }
                else {
                    // todo : expired
                    Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

                    this->showError(
                        resourceLoader.GetString(L"lp_demoapp_setting_popup1/Text"),
                        resourceLoader.GetString(L"lp_demoapp_setting_popup2/Text"),
                        resourceLoader.GetString(L"lp_demoapp_setting_popup3/Text")
                    );
                }
            }
        });
    }

    void SettingsPage::clickResetNow(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        showIntializeDialog();
    }

    void SettingsPage::userIdTextChanging(Microsoft::UI::Xaml::Controls::TextBox const& sender, Microsoft::UI::Xaml::Controls::TextBoxTextChangingEventArgs const& args) {
        std::ignore = args;

        if (sender.Text().empty()) {
            return;
        }

        wchar_t value = *sender.Text().rbegin();

        if (UiUtils::isAvailableChar(value) == false) {
            std::wstring removed = sender.Text().c_str();
            removed.erase(removed.size() - 1);
            textBoxUserId().Text(removed.c_str());

            textBoxUserId().Select(textBoxUserId().Text().size(), 0);
        }
    }

    bool SettingsPage::checkMadatory() {
        if (m_settingViewModel.name().empty() || m_settingViewModel.userId().empty()) {
            return false;
        }

        return true;
    }

    void SettingsPage::navigateMainPage() {
        auto typeMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeMainPage, nullptr, slide);
        mainFrame.Content().as<MainPage>().parentWindow(m_parentWindow);
    }

    Windows::Foundation::IAsyncAction SettingsPage::showIntializeDialog() {
        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

        Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(XamlRoot());

        dialog.Title(box_value(resourceLoader.GetString(L"lp_demoapp_setting_popup5/text")));
        dialog.Content(box_value(resourceLoader.GetString(L"lp_demoapp_setting_popup6/text")));
        dialog.PrimaryButtonText(resourceLoader.GetString(L"lp_demoapp_setting_popup3/text"));
        dialog.SecondaryButtonText(resourceLoader.GetString(L"lp_demoapp_setting_popup4/text"));

        dialog.DefaultButton(Microsoft::UI::Xaml::Controls::ContentDialogButton::Primary);

        auto result = co_await dialog.ShowAsync();

        if (result == Microsoft::UI::Xaml::Controls::ContentDialogResult::Primary) {
            // stop long polling
            m_settingViewModel.resetUser();
        }
        else {
            // do nothing
        }
    }

    Windows::Foundation::IAsyncAction SettingsPage::showError(hstring title, hstring content, hstring buttonString) {
        LINEPlanetCall::AlertPage alertPage;

        alertPage.title(title);
        alertPage.content(content);
        alertPage.buttonString(buttonString);


        Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(XamlRoot());
        dialog.Content(alertPage);
        alertPage.dialog(dialog);

        co_await dialog.ShowAsync();
    }
}

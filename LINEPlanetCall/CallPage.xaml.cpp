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
#include "CallPage.xaml.h"
#if __has_include("CallPage.g.cpp")
#include "CallPage.g.cpp"
#endif
#include "PlanetKitManager.h"

#include "MainWindow.xaml.h"
#include "CallViewModel.h"
#include "AudioDeviceViewModel.h"

#include "UiUtils.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t CallPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CallPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow CallPage::parentWindow() {
        return m_parentWindow;
    }

    void CallPage::parentWindow(LINEPlanetCall::MainWindow const& parent) {
        m_parentWindow = parent;

        m_callViewModel = make<CallViewModel>(m_parentWindow.settingService(), m_parentWindow.callService());
    }

    LINEPlanetCall::CallViewModel CallPage::callViewModel() {
        return m_callViewModel;
    }

    void CallPage::clickBack(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto typeCallScenarioPage = winrt::xaml_typename<winrt::LINEPlanetCall::CallScenarioPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeCallScenarioPage, nullptr, slide);
        mainFrame.Content().as<CallScenarioPage>().parentWindow(m_parentWindow);
    }

    void CallPage::clickHome(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto typeMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        auto mainFrame = m_parentWindow.mainFrame();
        mainFrame.Navigate(typeMainPage, nullptr, slide);
        mainFrame.Content().as<MainPage>().parentWindow(m_parentWindow);
    }

    void CallPage::clickAudioCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (checkPeerId() == false) {
            return;
        }

        LINEPlanetCall::AudioDeviceViewModel audioDeviceViewModel = make<AudioDeviceViewModel>(m_parentWindow.audioDeviceService());
        auto makeCallOperation = m_callViewModel.makeCall(false, audioDeviceViewModel.inputDeviceUid(), audioDeviceViewModel.outputDeviceUid());

        makeCallOperation.Completed([this](Windows::Foundation::IAsyncOperation<ConnectResult> const& result, auto const& status) {
            if (status == Windows::Foundation::AsyncStatus::Completed) {
                if (result.GetResults().connect) {
                    // succeed to make call
                    // navigate audio call view
                    auto typeAudioCallPage = winrt::xaml_typename<winrt::LINEPlanetCall::AudioCallPage>();

                    Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
                    slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromBottom);

                    auto mainFrame = m_parentWindow.mainFrame();
                    mainFrame.Navigate(typeAudioCallPage, nullptr, slide);
                    mainFrame.Content().as<AudioCallPage>().parentWindow(m_parentWindow);
                    mainFrame.Content().as<AudioCallPage>().callViewModel(m_callViewModel);
                }
                else {
                    // failed to call
                    // show error message
                    m_parentWindow.connectFailed(result.GetResults());
                }
            }
        });
    }

    void CallPage::clickVideoCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (checkPeerId() == false) {
            return;
        }

        LINEPlanetCall::AudioDeviceViewModel audioDeviceViewModel = make<AudioDeviceViewModel>(m_parentWindow.audioDeviceService());
        auto makeCallOperation = m_callViewModel.makeCall(true, audioDeviceViewModel.inputDeviceUid(), audioDeviceViewModel.outputDeviceUid());

        makeCallOperation.Completed([this](Windows::Foundation::IAsyncOperation<ConnectResult> const& result, auto const& status) {
            if (status == Windows::Foundation::AsyncStatus::Completed) {
                if (result.GetResults().connect) {
                    // succeed to make call
                    // navigate audio call view
                    auto typeVideoCallPage = winrt::xaml_typename<winrt::LINEPlanetCall::VideoCallPage>();

                    Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
                    slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromBottom);

                    auto mainFrame = m_parentWindow.mainFrame();
                    mainFrame.Navigate(typeVideoCallPage, nullptr, slide);
                    mainFrame.Content().as<VideoCallPage>().parentWindow(m_parentWindow);
                    mainFrame.Content().as<VideoCallPage>().callViewModel(m_callViewModel);
                }
                else {
                    // failed to call
                    // show error message
                    m_parentWindow.connectFailed(result.GetResults());
                }
            }
        });
    }

    void CallPage::peerIdTextChanging(Microsoft::UI::Xaml::Controls::TextBox const& sender, Microsoft::UI::Xaml::Controls::TextBoxTextChangingEventArgs const& args) {
        std::ignore = args;

        if (sender.Text().empty()) {
            return;
        }

        wchar_t value = *sender.Text().rbegin();

        if (UiUtils::isAvailableChar(value) == false) {
            std::wstring removed = sender.Text().c_str();
            removed.erase(removed.size() - 1);
            inputPeerId().Text(removed.c_str());

            inputPeerId().Select(inputPeerId().Text().size(), 0);
        }
    }

    bool CallPage::checkPeerId() {
        if (m_callViewModel.peerId().empty() == true) {
            Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

            showAlert(
                resourceLoader.GetString(L"lp_demoapp_common_error_startfail0/text"),
                resourceLoader.GetString(L"lp_demoapp_common_error_startfail1/text"),
                resourceLoader.GetString(L"lp_demoapp_1to1_scenarios_basic_endcall3/text")
            );

            return false;
        }

        return true;
    }

    Windows::Foundation::IAsyncAction CallPage::showAlert(hstring title, hstring content, hstring buttonString) {
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

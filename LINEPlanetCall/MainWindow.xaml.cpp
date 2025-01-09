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

#include <winrt/Windows.Data.Xml.Dom.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Windowing.h>

#include <commctrl.h>

#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "MainWindowViewModel.h"
#include "CallViewModel.h"

#include "UiUtils.h"
#include "AlertMessagePage.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    LRESULT CALLBACK MinSizeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uidSubclass, DWORD_PTR refData) {
        std::ignore = uidSubclass;
        std::ignore = refData;

        switch (msg) {
        case WM_GETMINMAXINFO:
            MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;

            minMaxInfo->ptMinTrackSize.x = UiUtils::scaledSize(Constants::minWindowWidth);
            minMaxInfo->ptMinTrackSize.y = UiUtils::scaledSize(Constants::minWindowHeight);

            break;
        }

        return DefSubclassProc(hwnd, msg, wParam, lParam);
    }


    MainWindow::~MainWindow() {
        m_mainWindowViewModel.PropertyChanged(m_tokenMainWindowViewModel);
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::InitializeComponent() {
        MainWindowT::InitializeComponent();

        AppWindow().SetIcon(L"Assets/LinePlanet.ico");

        m_mainWindowViewModel = make<MainWindowViewModel>(m_settingService, m_callService, m_groupCallService);
        m_tokenMainWindowViewModel = m_mainWindowViewModel.PropertyChanged({ this, &MainWindow::mainWindowViewModelProperyChanged });

        // Resize window
        UiUtils::resizeWindow(AppWindow(), { 492, 650 });

        auto presenter = AppWindow().Presenter();
        auto overlappedPresenter = presenter.as<Microsoft::UI::Windowing::OverlappedPresenter>();
        overlappedPresenter.IsMaximizable(false);
        overlappedPresenter.IsMinimizable(false);

        auto windowNative{ this->m_inner.as<::IWindowNative>() };
        HWND hWnd{ 0 };
        windowNative->get_WindowHandle(&hWnd);
        SetWindowSubclass(hWnd, MinSizeProc, 0, 0);
                
        // Create and navigate main page on `mainFrame`.
        auto typeNameMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();
        mainFrame().Navigate(typeNameMainPage);
        mainFrame().Content().as<MainPage>().parentWindow(*this);
    }

    Microsoft::UI::Xaml::Controls::Frame MainWindow::mainFrame() {
        return frame();
    }

    LINEPlanetCall::SettingService MainWindow::settingService() {
        return m_settingService;
    }

    LINEPlanetCall::CallService MainWindow::callService() {
        return m_callService;
    }

    LINEPlanetCall::AudioDeviceService MainWindow::audioDeviceService() {
        return m_audioDeviceService;
    }

    LINEPlanetCall::CameraDeviceService MainWindow::cameraDeviceService() {
        return m_cameraDeviceService;
    }

    LINEPlanetCall::GroupCallService MainWindow::groupCallService() {
        return m_groupCallService;
    }

    void MainWindow::endCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_mainWindowViewModel.endCall();
    }

    void MainWindow::acceptCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_mainWindowViewModel.acceptCall();
    }

    void MainWindow::closedMainWindow(IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args) {
        std::ignore = sender;

        if (mainFrame().Content().try_as<GroupCallCameraPreviewPage>()) {
            // paging to GroupCallScenarioPage
            auto typeGroupCallScenarioPage = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallScenarioPage>();

            Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
            slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromBottom);

            mainFrame().Navigate(typeGroupCallScenarioPage, nullptr, slide);
            mainFrame().Content().as<GroupCallScenarioPage>().parentWindow(*this);

            auto cameraController = PlanetKit::PlanetKitManager::GetInstance()->GetCameraController();
            cameraController->StopCapture();

            m_groupCallService.clear();

            // Resize window
            UiUtils::resizeWindow(AppWindow(), { 492, 650 });

            AppWindow().Title(Constants::appTitle);

            args.Handled(true);
        }
        else if (mainFrame().Content().try_as<AudioCallPage>()) {
            auto audioCallPage = mainFrame().Content().as<AudioCallPage>();
            audioCallPage.endCall();

            args.Handled(true);
        }
        else if (mainFrame().Content().try_as<VideoCallPage>()) {
            auto videoCallPage = mainFrame().Content().as<VideoCallPage>();
            videoCallPage.endCall();

            args.Handled(true);
        }
        else if (mainFrame().Content().try_as<GroupCallPage>()) {
            auto groupCallPage = mainFrame().Content().as<GroupCallPage>();
            groupCallPage.leaveConference();

            args.Handled(true);
        }
        else {
            stopNotification();
        }
    }

    void MainWindow::sizeChangedMainWindow(IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowSizeChangedEventArgs const& args) {
        std::ignore = sender;
        m_mainWindowViewModel.frameHeight(args.Size().Height);
    }

    LINEPlanetCall::MainWindowViewModel MainWindow::mainWindowViewModel() {
        return m_mainWindowViewModel;
    }

    void MainWindow::endCall(int32_t disconnectReason) {
        // call is disconnected.
        // navigate main page.
        navigateMainPage();
        showEndCall(disconnectReason);
    }

    void MainWindow::connectFailed(ConnectResult connectResult) {
        if (connectResult.errorType == ErrorType::appError && connectResult.appFailedReason == AppFailedReason::expiredAccessToken) {
            // reset user id
            m_settingService.resetUser();
            m_cameraDeviceService.stopCamera();
            navigateMainPage();
        }

        showConnectFailed(connectResult);
    }

    void MainWindow::mainWindowViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"accountStatus") {
            if (m_mainWindowViewModel.accountStatus() == AccountStatus::registered) {
                startNotification();
            }
            else {
                // stop notification
                stopNotification();
            }
        }
        else if (e.PropertyName() == L"callState") {
            switch (m_mainWindowViewModel.callState()) {
                case ConnectState::disconnected: {
                    if (m_mainWindowViewModel.accountStatus() == AccountStatus::registered) {
                        startNotification();
                    }

                    navigateMainPage();
                    showEndCall(m_mainWindowViewModel.callDisconnectReason());

                    break;
                }

                case ConnectState::connected: {
                    navigateCall();

                    break;
                }

                case ConnectState::waitAnswer: {
                    stopNotification();

                    break;
                }

                case ConnectState::verified: {
                    if (m_mainWindowViewModel.mediaType() == PlanetKitMediaType::audioVideo) {
                        // video call
                        navigateVerifiedVideoCallPage();
                    }
                    else {
                        // audio call
                    }

                    break;
                }
            }
        }
        else if (e.PropertyName() == L"groupCallState") {
            switch (m_mainWindowViewModel.groupCallState()) {
                case ConnectState::disconnected: {
                    if (m_mainWindowViewModel.accountStatus() == AccountStatus::registered) {
                        startNotification();
                    }

                    navigateMainPage();

                    break;
                }

                case ConnectState::connected: {
                    stopNotification();
                    break;
                }
            }
        }
    }

    void MainWindow::navigateMainPage() {
        auto mainPage = mainFrame().Content().try_as<MainPage>();
        if (mainPage != nullptr) {
            return;
        }

        auto typeMainPage = winrt::xaml_typename<winrt::LINEPlanetCall::MainPage>();

        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromLeft);

        mainFrame().Navigate(typeMainPage, nullptr, slide);
        mainFrame().Content().as<MainPage>().parentWindow(*this);

        // Resize window
        UiUtils::resizeWindow(AppWindow(), { 492, 650 });
    }

    void MainWindow::navigateVerifiedVideoCallPage() {
        auto typeNameVerified = xaml_typename<LINEPlanetCall::VerifiedVideoCallPage>();
        mainFrame().Navigate(typeNameVerified);
        mainFrame().Content().as<VerifiedVideoCallPage>().parentWindow(*this);

        // Resize window
        UiUtils::resizeWindow(AppWindow(), { 520, 460 });
    }

    Windows::Foundation::IAsyncAction MainWindow::showEndCall(int32_t disconnectReason) {
        // alert page
        LINEPlanetCall::AlertMessagePage alertMessagePage;

        alertMessagePage.disconnectReason(disconnectReason);

        Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(mainWindowPage().XamlRoot());
        dialog.Content(alertMessagePage);
        alertMessagePage.dialog(dialog);

        co_await dialog.ShowAsync();
    }

    Windows::Foundation::IAsyncAction MainWindow::showConnectFailed(ConnectResult connectResult) {
        // alert page
        LINEPlanetCall::AlertMessagePage alertMessagePage;

        if (connectResult.errorType == ErrorType::appError) {
            alertMessagePage.appFailedReason(connectResult.appFailedReason);
        }
        else {
            alertMessagePage.planetKitFailedReason(connectResult.planetKitFailedReason);
        }

        Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.XamlRoot(mainWindowPage().XamlRoot());
        dialog.Content(alertMessagePage);
        alertMessagePage.dialog(dialog);

        co_await dialog.ShowAsync();
    }

    void MainWindow::startNotification() {
        // call notification
        auto longPollingResult = m_mainWindowViewModel.notification();

        longPollingResult.Completed([this](Windows::Foundation::IAsyncOperation<NotificationResult> const& result, auto const& status) {
            if (status == Windows::Foundation::AsyncStatus::Completed) {
                auto notificationResult = result.GetResults();

                if (notificationResult.statusCode == winrt::Windows::Web::Http::HttpStatusCode::Ok) {
                    // verified call.
                    m_mainWindowViewModel.verifyCall(notificationResult.peerId, notificationResult.ccParam, notificationResult.mediaType, m_audioDeviceService.inputDeviceUid(), m_audioDeviceService.outputDeviceUid());
                }
                else {
                    // do nothing.
                }
            }
        });
    }

    void MainWindow::stopNotification() {
        m_mainWindowViewModel.stopNotification();
    }

    void MainWindow::navigateCall() {
        if (m_mainWindowViewModel.mediaType() == PlanetKitMediaType::audioVideo) {
            // video call
            auto page = mainFrame().Content().try_as<VideoCallPage>();
            if (page) {
                // already one to one call page
            }
            else {
                auto callViewModel = make<CallViewModel>(settingService(), callService());
                auto typeVideoCallPage = winrt::xaml_typename<winrt::LINEPlanetCall::VideoCallPage>();

                Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
                slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

                mainFrame().Navigate(typeVideoCallPage, nullptr, slide);
                mainFrame().Content().as<VideoCallPage>().parentWindow(*this);
                mainFrame().Content().as<VideoCallPage>().callViewModel(callViewModel);
            }
        }
        else {
            // audio call
            auto page = mainFrame().Content().try_as<AudioCallPage>();
            if (page) {
                // already one to one call page
            }
            else {
                auto callViewModel = make<CallViewModel>(settingService(), callService());
                auto typeAudioCallPage = winrt::xaml_typename<winrt::LINEPlanetCall::AudioCallPage>();

                Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
                slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

                mainFrame().Navigate(typeAudioCallPage, nullptr, slide);
                mainFrame().Content().as<AudioCallPage>().parentWindow(*this);
                mainFrame().Content().as<AudioCallPage>().callViewModel(callViewModel);
            }
        }
    }
}

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
#include "GroupCallCameraPreviewPage.xaml.h"
#if __has_include("GroupCallCameraPreviewPage.g.cpp")
#include "GroupCallCameraPreviewPage.g.cpp"
#endif

#include "UiUtils.h"
#include "GroupCallCameraPreviewViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t GroupCallCameraPreviewPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void GroupCallCameraPreviewPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    GroupCallCameraPreviewPage::~GroupCallCameraPreviewPage() {
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->DeregisterMyVideoReceiver(&m_videoRender);
        m_groupCallCameraPreviewViewModel.PropertyChanged(m_viewModelEventToken);
    }

    void GroupCallCameraPreviewPage::InitializeComponent() {
        GroupCallCameraPreviewPageT::InitializeComponent();

        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel swapChainPanel = previewPanel();
        m_videoRender.setSwapChainPanel(swapChainPanel);

        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->RegisterMyVideoReceiver(&m_videoRender);
    }

    LINEPlanetCall::MainWindow GroupCallCameraPreviewPage::parentWindow() {
        return m_parentWindow;
    }

    void GroupCallCameraPreviewPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        m_groupCallCameraPreviewViewModel = make<GroupCallCameraPreviewViewModel>(m_parentWindow.settingService(), m_parentWindow.groupCallService(), m_parentWindow.audioDeviceService());
        m_viewModelEventToken = m_groupCallCameraPreviewViewModel.PropertyChanged({ this, &GroupCallCameraPreviewPage::viewModelProperyChanged });

        m_parentWindow.AppWindow().Title(Constants::cameraPreview);
    }

    LINEPlanetCall::GroupCallCameraPreviewViewModel GroupCallCameraPreviewPage::groupCallCameraPreviewViewModel() {
        return m_groupCallCameraPreviewViewModel;
    }

    void GroupCallCameraPreviewPage::sizeChangedPreview(IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        float_t width = (float_t)previewPanel().ActualWidth();
        float_t height = (float_t)previewPanel().ActualHeight();

        m_videoRender.updateSize(width, height);

        if (m_groupCallCameraPreviewViewModel.myVideoEnabled() == false) {
            m_videoRender.drawBackground();
        }
    }

    void GroupCallCameraPreviewPage::clickMuteMyAudio(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_groupCallCameraPreviewViewModel.muteMyAudio();
    }

    void GroupCallCameraPreviewPage::clickVideoEnabled(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (m_groupCallCameraPreviewViewModel.myVideoEnabled() == true) {
            m_groupCallCameraPreviewViewModel.pauseMyVideo();
        }
        else {
            m_groupCallCameraPreviewViewModel.resumeMyVideo();
        }
    }

    void GroupCallCameraPreviewPage::clickEnterRoom(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        auto joinConferenceOperation = m_groupCallCameraPreviewViewModel.joinConference();

        joinConferenceOperation.Completed([this](Windows::Foundation::IAsyncOperation<ConnectResult> const& result, auto const& status) {
            if (status == Windows::Foundation::AsyncStatus::Completed) {
                if (result.GetResults().connect) {
                    auto typeGroupCallPage = winrt::xaml_typename<winrt::LINEPlanetCall::GroupCallPage>();

                    Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;

                    auto mainFrame = m_parentWindow.mainFrame();
                    mainFrame.Navigate(typeGroupCallPage, nullptr, slide);
                    mainFrame.Content().as<GroupCallPage>().parentWindow(m_parentWindow);

                    UiUtils::resizeWindow(m_parentWindow.AppWindow(), { 1000, 610 });
                }
                else {
                    // failed to joinConference
                    // failed to call
                    // show error message
                    m_parentWindow.connectFailed(result.GetResults());
                }
            }
            else {
                // failed to joinConference
            }
        });
    }

    void GroupCallCameraPreviewPage::viewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            bool paused = !m_groupCallCameraPreviewViewModel.myVideoEnabled();
            m_videoRender.paused(paused);

            if (paused) {
                m_videoRender.drawBackground();
            }
        }
    }
}

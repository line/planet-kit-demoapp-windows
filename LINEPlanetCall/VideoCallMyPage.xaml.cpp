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
#include "VideoCallMyPage.xaml.h"
#if __has_include("VideoCallMyPage.g.cpp")
#include "VideoCallMyPage.g.cpp"
#endif

#include "CallMyViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    VideoCallMyPage::~VideoCallMyPage() {
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->DeregisterMyVideoReceiver(&m_videoRender);
        m_callMyViewModel.PropertyChanged(m_tokenCallMyViewModel);
    }

    int32_t VideoCallMyPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void VideoCallMyPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void VideoCallMyPage::InitializeComponent() {
        VideoCallMyPageT::InitializeComponent();

        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel swapChainPanel = renderPanel();
        m_videoRender.setSwapChainPanel(swapChainPanel);

        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->RegisterMyVideoReceiver(&m_videoRender);
    }

    LINEPlanetCall::MainWindow VideoCallMyPage::parentWindow() {
        return m_parentWindow;
    }

    void VideoCallMyPage::parentWindow(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;
        m_callMyViewModel = make<CallMyViewModel>(parentWindow.callService());
        m_tokenCallMyViewModel = m_callMyViewModel.PropertyChanged({ this, &VideoCallMyPage::callMyViewModelProperyChanged });
    }

    LINEPlanetCall::CallMyViewModel VideoCallMyPage::callMyViewModel() {
        return m_callMyViewModel;
    }

    void VideoCallMyPage::sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        float_t width = (float_t)renderPanel().ActualWidth();
        float_t height = (float_t)renderPanel().ActualHeight();

        m_videoRender.updateSize(width, height);

        if (m_callMyViewModel.videoEnabled() == false) {
            m_videoRender.drawBackground();
        }
    }

    void VideoCallMyPage::callMyViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"videoEnabled") {
            bool paused = !m_callMyViewModel.videoEnabled();
            m_videoRender.paused(paused);

            if (paused) {
                m_videoRender.drawBackground();
            }
        }
    }
}

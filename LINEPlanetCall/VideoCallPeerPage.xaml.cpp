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
#include "VideoCallPeerPage.xaml.h"
#if __has_include("VideoCallPeerPage.g.cpp")
#include "VideoCallPeerPage.g.cpp"
#endif

#include "CallPeerViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    VideoCallPeerPage::~VideoCallPeerPage() {
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->DeregisterPeersVideoReceiver(&m_videoRender);
        m_callPeerViewModel.PropertyChanged(m_callPeerViewModelToken);
    }

    int32_t VideoCallPeerPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void VideoCallPeerPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void VideoCallPeerPage::InitializeComponent() {
        VideoCallPeerPageT::InitializeComponent();

        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel swapChainPanel = renderPanel();
        m_videoRender.setSwapChainPanel(swapChainPanel);

        m_videoRender.mirrorType(PlanetKit::EVideoMirrorType::PLNK_MIRROR_TYPE_UNMIRRORED);

        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->RegisterPeersVideoReceiver(&m_videoRender);
    }

    LINEPlanetCall::MainWindow VideoCallPeerPage::parentWindow() {
        return m_parentWindow;
    }

    void VideoCallPeerPage::parentWindow(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;
        m_callPeerViewModel = make<CallPeerViewModel>(parentWindow.callService());
        m_callPeerViewModelToken = m_callPeerViewModel.PropertyChanged({ this, &VideoCallPeerPage::callPeerViewModelProperyChanged });
    }

    LINEPlanetCall::CallPeerViewModel VideoCallPeerPage::callPeerViewModel() {
        return m_callPeerViewModel;
    }

    void VideoCallPeerPage::sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        float_t width = (float_t)renderPanel().ActualWidth();
        float_t height = (float_t)renderPanel().ActualHeight();

        m_videoRender.updateSize(width, height);

        // if video is not played.
        if (m_callPeerViewModel.videoEnabled() == false) {
            m_videoRender.drawBackground();
        }
    }

    void VideoCallPeerPage::callPeerViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"videoEnabled") {
            bool paused = !m_callPeerViewModel.videoEnabled();
            m_videoRender.paused(paused);

            if (paused) {
                m_videoRender.drawBackground();
            }
        }
    }
}

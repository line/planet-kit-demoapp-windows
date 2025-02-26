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
#include "VerifiedVideoCallPage.xaml.h"
#if __has_include("VerifiedVideoCallPage.g.cpp")
#include "VerifiedVideoCallPage.g.cpp"
#endif

#include "VerifiedCallViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    VerifiedVideoCallPage::~VerifiedVideoCallPage() {
        m_verifiedCallViewModel.PropertyChanged(m_verifiedCallViewModelEventToken);
        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->DeregisterMyVideoReceiver(&m_videoRender);
    }

    int32_t VerifiedVideoCallPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void VerifiedVideoCallPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void VerifiedVideoCallPage::InitializeComponent() {
        VerifiedVideoCallPageT::InitializeComponent();

        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel swapChainPanel = renderPanel();
        m_videoRender.setSwapChainPanel(swapChainPanel);

        PlanetKit::PlanetKitManager::GetInstance()->GetCameraController()->RegisterMyVideoReceiver(&m_videoRender);
    }

    LINEPlanetCall::MainWindow VerifiedVideoCallPage::parentWindow() {
        return m_parentWindow;
    }

    void VerifiedVideoCallPage::parentWindow(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;

        m_verifiedCallViewModel = make<VerifiedCallViewModel>(m_parentWindow.callService());
        m_verifiedCallViewModelEventToken = m_verifiedCallViewModel.PropertyChanged({ this, &VerifiedVideoCallPage::verifiedCallViewModelProperyChanged });

    }

    LINEPlanetCall::VerifiedCallViewModel VerifiedVideoCallPage::verifiedCallViewModel() {
        return m_verifiedCallViewModel;
    }

    void VerifiedVideoCallPage::clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_verifiedCallViewModel.muteMyAudio();
    }

    void VerifiedVideoCallPage::clickVideoEnable(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (m_verifiedCallViewModel.myVideoEnabled() == true) {
            m_verifiedCallViewModel.pauseMyVideo();
        }
        else {
            m_verifiedCallViewModel.resumeMyVideo();
        }
    }

    void VerifiedVideoCallPage::sizeChanged(IInspectable const& sender, Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        float_t width = (float_t)renderPanel().ActualWidth();
        float_t height = (float_t)renderPanel().ActualHeight();

        m_videoRender.updateSize(width, height);

        if (m_verifiedCallViewModel.myVideoEnabled() == false) {
            m_videoRender.drawBackground();
        }
    }

    void VerifiedVideoCallPage::verifiedCallViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            bool paused = !m_verifiedCallViewModel.myVideoEnabled();
            m_videoRender.paused(paused);

            if (m_verifiedCallViewModel.myVideoEnabled() == false) {
                m_videoRender.drawBackground();
            }
        }
    }
}

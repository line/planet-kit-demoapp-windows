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
#include "VideoCallPage.xaml.h"
#if __has_include("VideoCallPage.g.cpp")
#include "VideoCallPage.g.cpp"
#endif

#include "UiUtils.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    VideoCallPage::~VideoCallPage() {
        m_callViewModel.PropertyChanged(m_callViewModelToken);
    }

    int32_t VideoCallPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void VideoCallPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow VideoCallPage::parentWindow() {
        return m_parentWindow;
    }

    void VideoCallPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        // Resize window
        UiUtils::resizeWindow(m_parentWindow.AppWindow(), { 1000, 610 });
    }

    LINEPlanetCall::CallViewModel VideoCallPage::callViewModel() {
        return m_callViewModel;
    }

    void VideoCallPage::callViewModel(LINEPlanetCall::CallViewModel callViewModel) {
        m_callViewModel = callViewModel;
        m_callViewModelToken = m_callViewModel.PropertyChanged({ this, &VideoCallPage::callModelProperyChanged });
    }

    void VideoCallPage::endCall() {
        m_callViewModel.endCall();
    }

    void VideoCallPage::clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_callViewModel.muteMyAudio();
    }

    void VideoCallPage::clickVideoEnable(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        if (m_callViewModel.myVideoEnabled() == true) {
            m_callViewModel.pauseMyVideo();
        }
        else {
            m_callViewModel.resumeMyVideo();
        }
    }

    void VideoCallPage::clickLeaveCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_callViewModel.endCall();
    }

    void VideoCallPage::callModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"callState") {
            // call state is changed.
            if (m_callViewModel.callState() == ConnectState::disconnected) {
                // deregister event
                m_callViewModel.PropertyChanged(m_callViewModelToken);
            }
        }
    }
}

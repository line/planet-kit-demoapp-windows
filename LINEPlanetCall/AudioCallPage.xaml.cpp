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
#include "AudioCallPage.xaml.h"
#if __has_include("AudioCallPage.g.cpp")
#include "AudioCallPage.g.cpp"
#endif

#include "AudioCallMyPage.xaml.h"
#include "AudioCallPeerPage.xaml.h"

#include "UiUtils.h"

#include "AudioDeviceViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    AudioCallPage::~AudioCallPage() {
        m_callViewModel.PropertyChanged(m_callViewModelToken);
    }

    int32_t AudioCallPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void AudioCallPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow AudioCallPage::parentWindow() {
        return m_parentWindow;
    }

    void AudioCallPage::parentWindow(LINEPlanetCall::MainWindow parent) {
        m_parentWindow = parent;

        // Resize window
        UiUtils::resizeWindow(m_parentWindow.AppWindow(), { 1000, 610 });
    }

    LINEPlanetCall::CallViewModel AudioCallPage::callViewModel() {
        return m_callViewModel;
    }

    void AudioCallPage::callViewModel(LINEPlanetCall::CallViewModel callViewModel) {
        m_callViewModel = callViewModel;
        m_callViewModelToken = m_callViewModel.PropertyChanged({ this, &AudioCallPage::callModelProperyChanged });
    }

    void AudioCallPage::endCall() {
        m_callViewModel.endCall();
    }

    void AudioCallPage::clickMute(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_callViewModel.muteMyAudio();
    }

    void AudioCallPage::clickLeaveCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_callViewModel.endCall();
    }

    void AudioCallPage::callModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
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

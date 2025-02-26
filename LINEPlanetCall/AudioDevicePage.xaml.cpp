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
#include "AudioDevicePage.xaml.h"
#if __has_include("AudioDevicePage.g.cpp")
#include "AudioDevicePage.g.cpp"
#endif

#include "AudioDeviceViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t AudioDevicePage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void AudioDevicePage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    LINEPlanetCall::MainWindow AudioDevicePage::parentWindow() {
        return m_parentWindow;
    }

    void AudioDevicePage::parentWindow(LINEPlanetCall::MainWindow mainWindow) {
        m_parentWindow = mainWindow;
        m_audioDeviceViewModel = make<AudioDeviceViewModel>(mainWindow.audioDeviceService(), m_parentWindow.callService(), m_parentWindow.groupCallService());
        m_audioDeviceViewModel.callType(m_callType);
    }

    LINEPlanetCall::AudioDeviceViewModel AudioDevicePage::audioDeviceViewModel() {
        return m_audioDeviceViewModel;
    }

    CallType AudioDevicePage::callType() {
        return m_callType;
    }

    void AudioDevicePage::callType(CallType callType) {
        m_callType = callType;
    }

}

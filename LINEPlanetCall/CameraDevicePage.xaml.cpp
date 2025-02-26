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
#include "CameraDevicePage.xaml.h"
#if __has_include("CameraDevicePage.g.cpp")
#include "CameraDevicePage.g.cpp"
#endif

#include "CameraDeviceViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t CameraDevicePage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CameraDevicePage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    CallType CameraDevicePage::callType() {
        return m_callType;
    }

    void CameraDevicePage::callType(CallType callType) {
        m_callType = callType;
    }

    LINEPlanetCall::MainWindow CameraDevicePage::parentWindow() {
        return m_parentWindow;
    }

    void CameraDevicePage::parentWindow(LINEPlanetCall::MainWindow parentWindow) {
        m_parentWindow = parentWindow;
        m_cameraDeviceViewModel = make<CameraDeviceViewModel>(m_parentWindow.cameraDeviceService(), m_parentWindow.callService(), m_parentWindow.groupCallService());

        m_cameraDeviceViewModel.callType(m_callType);

        if (m_cameraDeviceViewModel.myVideoEnabled() == true) {
            m_cameraDeviceViewModel.startCamera();
        }
    }

    LINEPlanetCall::CameraDeviceViewModel CameraDevicePage::cameraDeviceViewModel() {
        return m_cameraDeviceViewModel;
    }

}

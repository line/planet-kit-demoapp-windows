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

#pragma once

#include "CameraDevicePage.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct CameraDevicePage : CameraDevicePageT<CameraDevicePage>
    {
        CameraDevicePage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        CallType callType();
        void callType(CallType callType);

        LINEPlanetCall::MainWindow parentWindow();
        void parentWindow(LINEPlanetCall::MainWindow parentWindow);

        LINEPlanetCall::CameraDeviceViewModel cameraDeviceViewModel();

    private:
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };
        LINEPlanetCall::CameraDeviceViewModel m_cameraDeviceViewModel{ nullptr };

        CallType m_callType = CallType::idle;
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct CameraDevicePage : CameraDevicePageT<CameraDevicePage, implementation::CameraDevicePage>
    {
    };
}

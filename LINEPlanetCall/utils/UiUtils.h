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

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Windowing.h>

#include "winrt/LINEPlanetCall.h"
#include "PlanetKitManager.h"

using winrt::Microsoft::UI::Windowing::AppWindow;
using winrt::Windows::Graphics::SizeInt32;

namespace winrt::LINEPlanetCall::implementation {
    namespace UiUtils {
        /**
         * Resize window.
         * @param appWindow `AppWindow` of target window.
         * @param size Width and height.
         */
        void resizeWindow(AppWindow appWindow, SizeInt32 size);

        int32_t scaledSize(int32_t size);

        PlanetKitAudioDeviceType audioDeviceType(PlanetKit::EAudioDeviceType type);

        bool isAvailableChar(wchar_t source);
    };
}

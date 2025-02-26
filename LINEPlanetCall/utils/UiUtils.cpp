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
#include "UiUtils.h"

#include <shellscalingapi.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shcore.lib")

namespace winrt::LINEPlanetCall::implementation {
    namespace UiUtils {
        DEVICE_SCALE_FACTOR MonitorScale() {
            auto activeWindow = GetActiveWindow();
            HMONITOR monitor = MonitorFromWindow(activeWindow, MONITOR_DEFAULTTONEAREST);

            DEVICE_SCALE_FACTOR eFactor;
            ::GetScaleFactorForMonitor(monitor, &eFactor);

            return eFactor;
        }

        void resizeWindow(AppWindow appWindow, SizeInt32 size) {
            auto eScaleFactor = MonitorScale();

            SizeInt32 scaleForm = {
                (int32_t)(size.Width * eScaleFactor / 100),
                (int32_t)(size.Height * eScaleFactor / 100)
            };

            appWindow.ResizeClient(scaleForm);
        }

        int32_t scaledSize(int32_t size) {
            auto eScaleFactor = MonitorScale();

            return size * eScaleFactor / 100;
        }

        PlanetKitAudioDeviceType audioDeviceType(PlanetKit::EAudioDeviceType type) {
            return type == PlanetKit::EAudioDeviceType::PLNK_AUDIO_DEVICE_TYPE_INPUT ? PlanetKitAudioDeviceType::input : PlanetKitAudioDeviceType::output;
        }

        bool isAvailableChar(wchar_t source) {
            bool result = false;

            if (source >= L'0' && source <= L'9') {
                result = true;
            }
            else if (source >= L'a' && source <= L'z') {
                result = true;
            }
            else if (source >= L'A' && source <= L'Z') {
                result = true;
            }
            else if (source == L'-' || source == L'_') {
                result = true;
            }
            else {
                result = false;
            }

            return result;
        }
    };
}
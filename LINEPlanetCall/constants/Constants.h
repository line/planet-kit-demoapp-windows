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

namespace winrt::LINEPlanetCall::implementation {
    namespace Constants {
        constexpr int32_t volumeCheck = 15;
        constexpr wchar_t appTitle[] = L"LINE Planet Call";
        constexpr wchar_t cameraPreview[] = L"Camera preview";

        constexpr wchar_t audioDeviceSystemDefaultUid[] = L"{System-Default}";

        constexpr int32_t connectedChecker = 1000;
        constexpr int32_t alertTimer = 5000;

        constexpr double_t minParticipantWidth = 80.0f;
        constexpr double_t minParticipantHeight = 45.0f;

        constexpr int32_t minWindowWidth = 360;
        constexpr int32_t minWindowHeight = 290;

        static std::wstring replaceMicString{L"{{mic name}}"};
    }
};
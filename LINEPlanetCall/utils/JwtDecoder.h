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
    class JwtDecoder {
    public:
        JwtDecoder(std::wstring const& jwt);

        std::wstring header();
        std::wstring payload();
        std::wstring signature();

    private:
        void split(std::wstring const& jwt);
        void decode(std::wstring& str);

    private:
        const std::wstring base64_chars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::wstring m_header;
        std::wstring m_payload;
        std::wstring m_signature;
    };
}

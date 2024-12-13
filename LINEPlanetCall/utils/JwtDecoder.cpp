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
#include "JwtDecoder.h"

namespace winrt::LINEPlanetCall::implementation {
    JwtDecoder::JwtDecoder(std::wstring const& jwt) {
        split(jwt);

        decode(m_header);
        decode(m_payload);
        decode(m_signature);
    }

    std::wstring JwtDecoder::header() {
        return m_header;
    }

    std::wstring JwtDecoder::payload() {
        return m_payload;
    }

    std::wstring JwtDecoder::signature() {
        return m_signature;
    }

    void JwtDecoder::split(std::wstring const& jwt) {
        std::wstring part;

        size_t pos = jwt.find(L'.');
        size_t rpos = jwt.rfind(L'.');

        m_header = jwt.substr(0, pos);
        m_payload = jwt.substr(pos + 1, rpos - pos - 1);
        m_signature = jwt.substr(rpos + 1);
    }

    void JwtDecoder::decode(std::wstring& str) {
        std::wstring decoded;

        // Base64URL -> Base64
        for (wchar_t& c : str) {
            if (c == L'-') {
                c = L'+';
            }
            else if (c == L'_') {
                c = L'/';
            }
        }

        while (str.size() % 4 != 0) {
            str += L'=';
        }

        uint64_t val = 0;
        int32_t valb = -8;

        for (wchar_t c : str) {
            if (base64_chars.find(c) == std::wstring::npos) {
                break;
            }

            val = (val << 6) + base64_chars.find(c);
            valb += 6;

            if (valb >= 0) {
                decoded.push_back(static_cast<wchar_t>((val >> valb) & 0xFF));
                valb -= 8;
            }
        }

        str = decoded;
    }
}
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

#include <mutex>

#include <d2d1_1.h>
#include <d2d1.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <d2d1helper.h>
#include <wincodec.h>

#include <atomic>

#include "PlanetKitManager.h"

class VideoRender final : public PlanetKit::IVideoReceiver {
public:
    VideoRender();
    virtual ~VideoRender();

    void OnVideo(const PlanetKit::SVideoFrame* pVideoFrame, PlanetKit::UserIdPtr pUserID) final;

    void setSwapChainPanel(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel& panel);

    PlanetKit::EVideoMirrorType mirrorType();
    void mirrorType(PlanetKit::EVideoMirrorType mirrorType);

    HRESULT drawBackground();
    
    void paused(bool paused);

    void updateSize(float_t width, float_t height);
    
private:
    struct RGB {
        int r;
        int g;
        int b;
    } m_rgb = { 0, 0, 0 };

    HRESULT createDeviceIndependentResources();
    HRESULT createDeviceResources();
    HRESULT createBitmap(PVOID pvBuffer, uint32_t width, uint32_t height, ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap);
    HRESULT createBackgroundBitmap(ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap, RGB rgb);

    winrt::Windows::Graphics::SizeInt32 desktopResolution();

    bool Rotate(PlanetKit::EVideoRotation eRotate, const PlanetKit::SVideoFrame* pVideoFrame, uint32_t& unResultWidth, uint32_t& unResultHeight);

    HRESULT drawFrame(PVOID pvBuffer, uint32_t width, uint32_t height);
    bool mirrorFrame(PBYTE pvBuffer, uint32_t unBufferSize, int32_t nWidth, int32_t nHeight);

private:
    std::recursive_mutex m_mutex;

    winrt::com_ptr<IDXGISwapChain1> m_pSwapChain;
    winrt::com_ptr<ID2D1DeviceContext> m_d2dDeviceContext;
    IWICImagingFactory2* m_wicFactory = nullptr;
    ID2D1Bitmap1* m_videoBitmap = nullptr;

    float_t m_width = 0.0f;
    float_t m_height = 0.0f;

    std::vector<BYTE> m_frameBuffer;
    std::vector<BYTE> m_frameBufferMirror;

    std::atomic<bool> m_paused = false;

    PlanetKit::EVideoMirrorType m_mirrorType = PlanetKit::EVideoMirrorType::PLNK_MIRROR_TYPE_MIRRORED;
};

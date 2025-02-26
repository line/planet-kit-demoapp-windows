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

#include <microsoft.ui.xaml.media.dxinterop.h>

#include "VideoRender.h"
#include "libyuv.h"

using LockGuard = std::lock_guard<std::recursive_mutex>;

template <class Interface> inline void SafeRelease(Interface** interfaceToRelease) {
    if (*interfaceToRelease != nullptr) {
        (*interfaceToRelease)->Release();
        (*interfaceToRelease) = nullptr;
    }
}

VideoRender::VideoRender() {
    createDeviceIndependentResources();
}

VideoRender::~VideoRender() {
    if (m_videoBitmap != nullptr) {
        m_videoBitmap->Release();
    }
}

void VideoRender::OnVideo(const PlanetKit::SVideoFrame* pVideoFrame, PlanetKit::UserIdPtr pUserID) {
    LockGuard lock(m_mutex);

    if (m_d2dDeviceContext == nullptr) {
        return;
    }
    else if (m_paused) {
        // draw background
        drawBackground();
        return;
    }

    HRESULT hr = S_OK;

    PVOID pvBuffer = pVideoFrame->pbuffer;
    uint32_t unWidth = pVideoFrame->unWidth;
    uint32_t unHeight = pVideoFrame->unHeight;

    if (pVideoFrame->eRotation != PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_0) {
        // rotate video frame
        if (Rotate(pVideoFrame->eRotation, pVideoFrame, unWidth, unHeight) == false) {
            return;
        }

        pvBuffer = m_frameBuffer.data();
    }

    // mirror frame
    if (m_mirrorType == PlanetKit::EVideoMirrorType::PLNK_MIRROR_TYPE_MIRRORED) {
        mirrorFrame((PBYTE)pvBuffer, pVideoFrame->unBufferSize, (int32_t)unWidth, (int32_t)unHeight);
        pvBuffer = m_frameBufferMirror.data();
    }

    hr = drawFrame(pvBuffer, unWidth, unHeight); // FALSE specifies drawing to screen.

    if (SUCCEEDED(hr)) {
        // Present the swap chain immediately
        hr = m_pSwapChain->Present(0, 0);
    }
}

void VideoRender::setSwapChainPanel(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel& panel) {
    LockGuard lock(m_mutex);

    if (panel == nullptr) {
        return;
    }

    m_width = (float_t)panel.ActualWidth();
    m_height = (float_t)panel.ActualHeight();

    uint32_t creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the Direct3D device.
    winrt::com_ptr<::ID3D11Device> d3dDevice;
    D3D_FEATURE_LEVEL supportedFeatureLevel;

    HRESULT hr = S_OK;
    std::wstring msg;

    D3D_DRIVER_TYPE tryDriverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
        D3D_DRIVER_TYPE_NULL,
        D3D_DRIVER_TYPE_SOFTWARE
    };

    for (int32_t idx = 0; idx < _countof(tryDriverTypes); ++idx) {
        hr = ::D3D11CreateDevice(
            nullptr,
            (D3D_DRIVER_TYPE)idx,
            0,
            creationFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            d3dDevice.put(),
            &supportedFeatureLevel,
            nullptr);

        if (SUCCEEDED(hr)) {
            break;
        }
    }

    if (FAILED(hr)) {
        return;
    }

    // Get the Direct3D device.
    winrt::com_ptr<::IDXGIDevice> dxgiDevice{
        d3dDevice.as<::IDXGIDevice>() };

    winrt::com_ptr<::ID2D1Device> d2dDevice;
    ::D2D1CreateDevice(dxgiDevice.get(), nullptr, d2dDevice.put());

    hr = d2dDevice->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        m_d2dDeviceContext.put()
    );

    if (FAILED(hr)) {
        return;
    }

    // Get the DXGI adapter.
    winrt::com_ptr<::IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(dxgiAdapter.put());

    // Get the DXGI factory.
    winrt::com_ptr< ::IDXGIFactory2 > dxgiFactory;
    dxgiFactory.capture(dxgiAdapter, &IDXGIAdapter::GetParent);

    auto size = desktopResolution();

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };

    // these two values mean max size.
    swapChainDesc.Width = size.Width;
    swapChainDesc.Height = size.Height;

    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swapchain format.
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // We recommend using this swap effect for all applications.
    swapChainDesc.Flags = 0;

    // Create a swap chain by calling IDXGIFactory2::CreateSwapChainForComposition.
    dxgiFactory->CreateSwapChainForComposition(
        d3dDevice.get(),
        &swapChainDesc,
        nullptr,
        m_pSwapChain.put());
    auto panelNative{ panel.as<ISwapChainPanelNative>() };

    winrt::hresult result{ panelNative->SetSwapChain(m_pSwapChain.get()) };
    winrt::check_hresult(result);

    hr = createDeviceResources();

    if (FAILED(hr)) {
        
    }
}

PlanetKit::EVideoMirrorType VideoRender::mirrorType() {
    return m_mirrorType;
}

void VideoRender::mirrorType(PlanetKit::EVideoMirrorType mirrorType) {
    m_mirrorType = mirrorType;
}

HRESULT VideoRender::drawBackground() {
    HRESULT hr = S_OK;

    ID2D1Bitmap* pD2DBackgroundBitmap = nullptr;
    ID2D1DeviceContext* d2dContext = m_d2dDeviceContext.get();

    hr = createBackgroundBitmap(d2dContext, &pD2DBackgroundBitmap, m_rgb);

    if (FAILED(hr)) {
        return hr;
    }

    // Sometimes the background becomes smaller by 1 pixel, adding correction value.
    D2D1_RECT_F destBackgroundRect = { 0.0f, 0.0f, m_width + 1.0f, m_height + 1.0f };
    D2D1_RECT_F destSource = { 0.0f, 0.0f, 1.0f, 1.0f };

    d2dContext->BeginDraw();

    try {
        // draw background
        d2dContext->DrawBitmap(
            pD2DBackgroundBitmap,
            &destBackgroundRect,
            1,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            &destSource
        );
    }
    catch (const std::exception& e) {
        std::ignore = e;
    };

    hr = d2dContext->EndDraw();

    pD2DBackgroundBitmap->Release();

    if (SUCCEEDED(hr)) {
        // Present the swap chain immediately
        hr = m_pSwapChain->Present(0, 0);
    }

    return hr;
}

void VideoRender::paused(bool paused) {
    m_paused = paused;
}

void VideoRender::updateSize(float_t width, float_t height) {
    LockGuard lock(m_mutex);

    m_width = width;
    m_height = height;
}

HRESULT VideoRender::createDeviceIndependentResources() {
    HRESULT hr = S_OK;

    if (SUCCEEDED(hr)) {
        // Create a Direct2D factory.
        D2D1_FACTORY_OPTIONS options;
        ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
        // If the project is in a debug build, enable Direct2D debugging via SDK Layers
        options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    }
    if (SUCCEEDED(hr)) {
        // Create a WIC factory.
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
        );
    }

    return hr;
}

HRESULT VideoRender::createDeviceResources() {
    HRESULT hr = S_OK;

    if (m_pSwapChain == nullptr) {
        return S_FALSE;
    }

    if (m_videoBitmap != nullptr) {
        m_videoBitmap->Release();
        m_videoBitmap = nullptr;
    }

    IDXGISurface* surface = nullptr;
    if (SUCCEEDED(hr)) {
        // Get a surface from the swap chain.
        hr = m_pSwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&surface)
        );
    }

    if (SUCCEEDED(hr)) {
        // Create a bitmap pointing to the surface.
        D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_IGNORE
            )
        );

        hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
            surface,
            &properties,
            &m_videoBitmap
        );
    }
    if (SUCCEEDED(hr)) {
        // Set the bitmap as the target of our device context.
        m_d2dDeviceContext->SetTarget(m_videoBitmap);
    }

    SafeRelease(&surface);


    return hr;
}

HRESULT VideoRender::createBitmap(PVOID pvBuffer, uint32_t width, uint32_t height, ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap) {
    HRESULT hr;
    IWICBitmap* pBitmap = nullptr;

    std::vector<BYTE> brga_buffer((width * height * 4));

    libyuv::I420ToARGB((const uint8_t*)pvBuffer, width,
        (const uint8_t*)pvBuffer + width * height, width / 2,
        (const uint8_t*)pvBuffer + width * height * 5 / 4, width / 2,
        brga_buffer.data(), width * 4,
        width, height);

    uint32_t unStride = width * 4;
    uint32_t unSize = width * height * 4;

    hr = m_wicFactory->CreateBitmapFromMemory(width, height, GUID_WICPixelFormat32bppPBGRA, unStride, unSize, brga_buffer.data(), &pBitmap);

    if (SUCCEEDED(hr)) {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = d2dContext->CreateBitmapFromWicBitmap(
            pBitmap,
            nullptr,
            bitmap
        );

        pBitmap->Release();
    }
    return hr;
}

HRESULT VideoRender::createBackgroundBitmap(ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap, RGB rgb) {
    HRESULT hr;
    IWICBitmap* pBitmap = nullptr;

    constexpr int nPixel = 1;

    std::vector<BYTE> brga_buffer(nPixel * nPixel * 4);

    for (int y = 0; y < nPixel; y++) {
        for (int x = 0; x < nPixel; x++) {
            // BGRA order
            int bgra_offset = (y * nPixel + x) * 4;
            brga_buffer[bgra_offset] = static_cast<unsigned char>(rgb.b);
            brga_buffer[bgra_offset + 1] = static_cast<unsigned char>(rgb.g);
            brga_buffer[bgra_offset + 2] = static_cast<unsigned char>(rgb.r);
            brga_buffer[bgra_offset + 3] = 255; // Alpha channel, set to 255 (fully opaque)
        }
    }

    uint32_t unStride = nPixel * 4;
    uint32_t unSize = nPixel * nPixel * 4;

    hr = m_wicFactory->CreateBitmapFromMemory(nPixel, nPixel, GUID_WICPixelFormat32bppPBGRA, unStride, unSize, brga_buffer.data(), &pBitmap);

    if (SUCCEEDED(hr)) {
        try {
            // Create a Direct2D bitmap from the WIC bitmap.
            hr = d2dContext->CreateBitmapFromWicBitmap(
                pBitmap,
                nullptr,
                bitmap
            );
        }
        catch (const std::exception& e) {
            std::ignore = e;
        };


        pBitmap->Release();
    }
    return hr;
}

winrt::Windows::Graphics::SizeInt32 VideoRender::desktopResolution() {
    RECT rtDesktop;

    const HWND hDesktop = ::GetDesktopWindow();
    GetWindowRect(hDesktop, &rtDesktop);

    return winrt::Windows::Graphics::SizeInt32{ rtDesktop.right, rtDesktop.bottom };
}

bool VideoRender::Rotate(PlanetKit::EVideoRotation eRotate, const PlanetKit::SVideoFrame* pVideoFrame, uint32_t& unResultWidth, uint32_t& unResultHeight) {
    if (m_frameBuffer.size() < pVideoFrame->unBufferSize) {
        m_frameBuffer.resize(pVideoFrame->unBufferSize, 0);
    }

    if (m_frameBuffer.size() == 0) {
        return false;
    }

    uint32_t width = pVideoFrame->unWidth;
    uint32_t height = pVideoFrame->unHeight;

    const uint8_t* src_yplane = pVideoFrame->pbuffer;
    const uint8_t* src_uplane = src_yplane + width * height;
    const uint8_t* src_vplane = src_uplane + (width * height / 4);

    uint8_t* dst_yplane = m_frameBuffer.data();
    uint8_t* dst_uplane = dst_yplane + width * height;
    uint8_t* dst_vplane = dst_uplane + (width * height / 4);

    libyuv::I420Rotate(
        src_yplane, width,
        src_uplane, width / 2,
        src_vplane, width / 2,
        dst_yplane, eRotate == PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_180 ? width : height,
        dst_uplane, (eRotate == PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_180 ? width : height) / 2,
        dst_vplane, (eRotate == PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_180 ? width : height) / 2,
        width, height, (libyuv::RotationMode)(eRotate * 90));

    unResultWidth = eRotate == PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_180 ? width : height;
    unResultHeight = eRotate == PlanetKit::EVideoRotation::PLNK_VIDEO_ROTATION_180 ? height : width;

    return true;
}

HRESULT VideoRender::drawFrame(PVOID pvBuffer, uint32_t width, uint32_t height) {
    HRESULT hr = S_OK;

    ID2D1Bitmap* pD2DBitmap = nullptr;
    ID2D1Bitmap* pD2DBackgroundBitmap = nullptr;
    ID2D1DeviceContext* d2dContext = m_d2dDeviceContext.get();

    hr = createBitmap(pvBuffer, width, height, d2dContext, &pD2DBitmap);
    if (FAILED(hr)) {
        return hr;
    }

    hr = createBackgroundBitmap(d2dContext, &pD2DBackgroundBitmap, m_rgb);

    if (FAILED(hr)) {
        pD2DBitmap->Release();
        return hr;
    }

    D2D1_RECT_F destBackgroundRect = { 0.0f, 0.0f, m_width, m_height };
    D2D1_RECT_F destRect = { 0.0f, 0.0f, 0.0f, 0.0f };

    D2D1_RECT_F sourceRect = { 0.0f, 0.0f, (float_t)width, (float_t)height };

    destRect.right = m_width;
    destRect.bottom = m_height;

    float_t fWidthRatio = m_width / (float_t)width;
    float_t fHeightRatio = m_height / (float_t)height;

    if (fWidthRatio > fHeightRatio) {
        float_t fScaledHeight = m_width / width * height;
        float_t fHeightOffset = (fScaledHeight - m_height) / 2.0f;

        if (fHeightOffset > 0.0f) {
            sourceRect.top = height * (fHeightOffset / fScaledHeight);
            sourceRect.bottom = height * ((fScaledHeight - fHeightOffset) / fScaledHeight);
        }
        else {
            // make as fill to window
            // do nothing
        }
    }
    else if (fWidthRatio < fHeightRatio) {
        float_t fScaledWidth = m_height / height * width;
        float_t fWidthOffset = (fScaledWidth - m_width) / 2.0f;

        if (fWidthOffset > 0.0f) {
            sourceRect.left = width * (fWidthOffset / fScaledWidth);
            sourceRect.right = width * ((fScaledWidth - fWidthOffset) / fScaledWidth);
        }
        else {
            // make as fill to window
            // do nothing
        }
    }

    d2dContext->BeginDraw();

    try {
        // draw background
        d2dContext->DrawBitmap(
            pD2DBackgroundBitmap,
            &destBackgroundRect,
            1,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            &sourceRect
        );

        // draw video stream
        d2dContext->DrawBitmap(
            pD2DBitmap,
            &destRect,
            1,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            &sourceRect
        );
    }
    catch (const std::exception& e) {
        std::ignore = e;
    };

    hr = d2dContext->EndDraw();

    pD2DBackgroundBitmap->Release();
    pD2DBitmap->Release();

    return hr;
}

bool VideoRender::mirrorFrame(PBYTE pvBuffer, uint32_t unBufferSize, int32_t nWidth, int32_t nHeight) {
    if (m_frameBufferMirror.size() < unBufferSize) {
        m_frameBufferMirror.resize(unBufferSize, 0);
    }

    const uint8_t* src_yplane = pvBuffer;
    const uint8_t* src_uplane = src_yplane + nWidth * nHeight;
    const uint8_t* src_vplane = src_uplane + (nWidth * nHeight / 4);

    uint8_t* dst_yplane = m_frameBufferMirror.data();
    uint8_t* dst_uplane = dst_yplane + nWidth * nHeight;
    uint8_t* dst_vplane = dst_uplane + (nWidth * nHeight / 4);

    return libyuv::I420Mirror(
        src_yplane, nWidth,
        src_uplane, nWidth / 2,
        src_vplane, nWidth / 2,
        dst_yplane, nWidth,
        dst_uplane, nWidth / 2,
        dst_vplane, nWidth / 2,
        nWidth, nHeight);
}
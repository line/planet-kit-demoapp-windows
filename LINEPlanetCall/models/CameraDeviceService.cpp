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

#include "CameraDeviceService.h"

#if __has_include("CameraDeviceService.g.cpp")
#include "CameraDeviceService.g.cpp"
#endif

#include "PlanetKitManager.h"

namespace winrt::LINEPlanetCall::implementation {
    CameraDeviceService::CameraDeviceService() {
        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto planetKitCameraController = planetKitManager->GetCameraController();

        planetKitCameraController->RegisterDeviceEvent(&m_cameraDeviceListener);

        PlanetKit::CameraInfoArray cameraInfos;
        planetKitCameraController->GetCapturerInfo(cameraInfos);

        for (int32_t idx = 0; idx < cameraInfos.Size(); ++idx) {
            CameraDevice device(cameraInfos[idx]->GetDeviceUid().c_str(), cameraInfos[idx]->GetDeviceName().c_str());
            m_cameras.Append(device);
        }
    }

    event_token CameraDeviceService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CameraDeviceService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Windows::Foundation::Collections::IObservableVector<CameraDevice> CameraDeviceService::cameras() {
        LockGuard lock(m_lock);
        return m_cameras;
    }

    int32_t CameraDeviceService::selectedDeviceIndex() {
        return m_selectedDeviceIndex;
    }

    void CameraDeviceService::selectedDeviceIndex(int32_t index) {
        LockGuard lock(m_lock);

        if (m_selectedDeviceIndex != index && index >= 0 && m_cameras.Size() > (uint32_t)index) {
            m_selectedDeviceIndex = index;

            auto uid = m_cameras.GetAt(index).uid();

            auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
            auto planetKitCameraController = planetKitManager->GetCameraController();
            PlanetKit::CameraInfoArray cameraInfos;
            planetKitCameraController->GetCapturerInfo(cameraInfos);

            for (int32_t idx = 0; idx < cameraInfos.Size(); ++idx) {
                if (cameraInfos[idx]->GetDeviceUid() == uid.c_str()) {
                    planetKitCameraController->SelectCamera(cameraInfos[idx]);
                }
            }

            dispatchPropertyChanged(L"selectedDeviceIndex");
        }
        else {
            m_selectedDeviceIndex = index;

            if (index == 0 && m_cameras.Size() == 0) {
                dispatchPropertyChanged(L"selectedDeviceIndex");
            }
        }
    }

    bool CameraDeviceService::startCamera() {
        if (m_selectedDeviceIndex < 0) {
            return false;
        }

        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto planetKitCameraController = planetKitManager->GetCameraController();

        if (planetKitCameraController->IsRunning()) {
            return true;
        }

        auto startResult = planetKitCameraController->StartCapture();

        if (startResult == PlanetKit::EVideoControlResult::EVideoControlResult_Success || startResult == PlanetKit::EVideoControlResult::EVideoControlResult_AlreadyRunning) {
            return true;
        }
        else {
            return false;
        }
    }

    void CameraDeviceService::stopCamera() {
        auto planetKitManager = PlanetKit::PlanetKitManager::GetInstance();
        auto planetKitCameraController = planetKitManager->GetCameraController();

        planetKitCameraController->StopCapture();
    }

    void CameraDeviceService::addedCameraDevice(hstring uid, hstring name) {
        LockGuard lock(m_lock);

        CameraDevice cameraDevice(uid, name);
        m_cameras.Append(cameraDevice);

        if (m_cameras.Size() == 1 && m_selectedDeviceIndex == -1) {
            selectedDeviceIndex(0);
        }
    }

    void CameraDeviceService::removedCameraDevice(hstring uid) {
        LockGuard lock(m_lock);

        for (int32_t idx = 0; idx < (int32_t)m_cameras.Size(); ++idx) {
            auto cam = m_cameras.GetAt(idx);

            if (cam.uid() == uid) {
                // found matching item
                // remove from the list
                if (m_selectedDeviceIndex == idx) {
                    m_selectedDeviceIndex = -1;

                    dispatchPropertyChanged(L"selectedDeviceIndex");
                }

                m_cameras.RemoveAt(idx);

                break;
            }
        }
    }
    
    void CameraDeviceService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
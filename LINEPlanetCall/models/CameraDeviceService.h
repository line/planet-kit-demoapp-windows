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

#include "CameraDeviceService.g.h"
#include "CameraDeviceListener.h"

#include <mutex>

namespace winrt::LINEPlanetCall::implementation {
    typedef std::recursive_mutex MutexLock;
    typedef std::lock_guard<MutexLock> LockGuard;

    struct CameraDeviceService : CameraDeviceServiceT<CameraDeviceService> {
        CameraDeviceService();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        Windows::Foundation::Collections::IObservableVector<CameraDevice> cameras();

        int32_t selectedDeviceIndex();
        void selectedDeviceIndex(int32_t index);

        bool startCamera();
        void stopCamera();

        void addedCameraDevice(hstring uid, hstring name);
        void removedCameraDevice(hstring uid);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        CameraDeviceListener m_cameraDeviceListener{ *this };

        MutexLock m_lock;

        Windows::Foundation::Collections::IObservableVector<CameraDevice> m_cameras = single_threaded_observable_vector<CameraDevice>();
        int32_t m_selectedDeviceIndex = 0;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CameraDeviceService : CameraDeviceServiceT<CameraDeviceService, implementation::CameraDeviceService> {

    };
}
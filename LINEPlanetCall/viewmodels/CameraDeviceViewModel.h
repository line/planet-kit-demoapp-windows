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

#include "CameraDeviceViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct CameraDeviceViewModel : CameraDeviceViewModelT<CameraDeviceViewModel> {
        CameraDeviceViewModel(LINEPlanetCall::CameraDeviceService cameraDeviceService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService);
        virtual ~CameraDeviceViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        Windows::Foundation::Collections::IObservableVector<CameraDevice> cameras();

        int32_t selectedDeviceIndex();
        void selectedDeviceIndex(int32_t index);

        CallType callType();
        void callType(CallType callType);

        bool myVideoEnabled();

        void startCamera();
        void stopCamera();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void cameraDeviceServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
        void callServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
        void groupCallServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::CameraDeviceService m_cameraDeviceService{ nullptr };
        LINEPlanetCall::CallService m_callService{ nullptr };
        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };

        CallType m_callType = CallType::idle;

        event_token m_cameraDeviceServiceEventToken;
        event_token m_callServiceEventToken;
        event_token m_groupCallServiceEventToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CameraDeviceViewModel : CameraDeviceViewModelT<CameraDeviceViewModel, implementation::CameraDeviceViewModel> {

    };
}
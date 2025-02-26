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

#include "CameraDeviceViewModel.h"

#if __has_include("CameraDeviceViewModel.g.cpp")
#include "CameraDeviceViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    CameraDeviceViewModel::CameraDeviceViewModel(LINEPlanetCall::CameraDeviceService cameraDeviceService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService) {
        m_cameraDeviceService = cameraDeviceService;
        m_cameraDeviceServiceEventToken = m_cameraDeviceService.PropertyChanged({ this, &CameraDeviceViewModel::cameraDeviceServicePropertyChanged });

        m_callService = callService;
        if (m_callService) {
            m_callServiceEventToken = m_callService.PropertyChanged({ this, &CameraDeviceViewModel::callServicePropertyChanged });
        }
        
        m_groupCallService = groupCallService;
        if (m_groupCallService) {
            m_groupCallServiceEventToken = m_groupCallService.PropertyChanged({ this, &CameraDeviceViewModel::groupCallServicePropertyChanged });
        }
    }

    CameraDeviceViewModel::~CameraDeviceViewModel() {
        if (m_callService) {
            m_callService.PropertyChanged(m_callServiceEventToken);
            m_callService = nullptr;
        }
        
        if (m_groupCallService) {
            m_groupCallService.PropertyChanged(m_groupCallServiceEventToken);
            m_groupCallService = nullptr;
        }

        m_cameraDeviceService.PropertyChanged(m_cameraDeviceServiceEventToken);
    }

    event_token CameraDeviceViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void CameraDeviceViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Windows::Foundation::Collections::IObservableVector<CameraDevice> CameraDeviceViewModel::cameras() {
        return m_cameraDeviceService.cameras();
    }

    int32_t CameraDeviceViewModel::selectedDeviceIndex() {
        return m_cameraDeviceService.selectedDeviceIndex();
    }

    void CameraDeviceViewModel::selectedDeviceIndex(int32_t index) {
        m_cameraDeviceService.selectedDeviceIndex(index);
    }

    CallType CameraDeviceViewModel::callType() {
        return m_callType;
    }

    void CameraDeviceViewModel::callType(CallType callType) {
        m_callType = callType;

        dispatchPropertyChanged(L"callType");
    }

    bool CameraDeviceViewModel::myVideoEnabled() {
        if (m_callType == CallType::call) {
            return m_callService.myVideoEnabled();
        }
        else if (m_callType == CallType::groupCall) {
            return m_groupCallService.myVideoEnabled();
        }

        return false;
    }

    void CameraDeviceViewModel::startCamera() {
        if (m_cameraDeviceService.startCamera() == false) {
            if (m_callType == CallType::call) {
                m_callService.pauseMyVideo();
            }
            else if (m_callType == CallType::groupCall) {
                m_groupCallService.pauseMyVideo();
            }
        }
    }

    void CameraDeviceViewModel::stopCamera() {
        m_cameraDeviceService.stopCamera();
    }
    
    void CameraDeviceViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void CameraDeviceViewModel::cameraDeviceServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"selectedDeviceIndex") {
            if (m_cameraDeviceService.selectedDeviceIndex() == -1 || m_cameraDeviceService.cameras().Size() == 0) {
                // selected device is removed.
                // or there is no device to select.

                if (m_callType == CallType::call) {
                    m_callService.pauseMyVideo();
                }
                else if (m_callType == CallType::groupCall) {
                    m_groupCallService.pauseMyVideo();
                }
            }
        }

        dispatchPropertyChanged(e.PropertyName());
    }

    void CameraDeviceViewModel::callServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            if (m_callService.myVideoEnabled() == true) {
                startCamera();
            }
            else {
                stopCamera();
            }
        }
    }

    void CameraDeviceViewModel::groupCallServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            if (m_groupCallService.myVideoEnabled() == true) {
                startCamera();
            }
            else {
                stopCamera();
            }
        }
    }
}
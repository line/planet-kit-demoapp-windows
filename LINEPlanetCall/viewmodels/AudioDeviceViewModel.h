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

#include "AudioDeviceViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct AudioDeviceViewModel : AudioDeviceViewModelT<AudioDeviceViewModel> {
        AudioDeviceViewModel(LINEPlanetCall::AudioDeviceService audioDeviceService);
        AudioDeviceViewModel(LINEPlanetCall::AudioDeviceService audioDeviceService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService);
        
        virtual ~AudioDeviceViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        CallType callType();
        void callType(CallType callType);

        Windows::Foundation::Collections::IObservableVector<AudioDevice> inputs();
        Windows::Foundation::Collections::IObservableVector<AudioDevice> outputs();

        int32_t selectedInputDeviceIndex();
        void selectedInputDeviceIndex(int32_t selectedIndex);

        int32_t selectedOutputDeviceIndex();
        void selectedOutputDeviceIndex(int32_t selectedIndex);

        hstring inputDeviceUid();
        hstring outputDeviceUid();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void audioDevicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        bool isCallActive(ConnectState callState);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        CallType m_callType = CallType::idle;

        LINEPlanetCall::CallService m_callService{ nullptr };
        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };

        LINEPlanetCall::AudioDeviceService m_audioDeviceService{ nullptr };
        event_token m_audioDeviceServiceToken;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct AudioDeviceViewModel : AudioDeviceViewModelT<AudioDeviceViewModel, implementation::AudioDeviceViewModel> {

    };
}
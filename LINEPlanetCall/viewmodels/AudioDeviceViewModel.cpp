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

#include "AudioDeviceViewModel.h"

#if __has_include("AudioDeviceViewModel.g.cpp")
#include "AudioDeviceViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    AudioDeviceViewModel::AudioDeviceViewModel(LINEPlanetCall::AudioDeviceService audioDeviceService) {
        m_audioDeviceService = audioDeviceService;

        m_audioDeviceServiceToken = m_audioDeviceService.PropertyChanged({ this, &AudioDeviceViewModel::audioDevicePropertyChanged });
    }

    AudioDeviceViewModel::AudioDeviceViewModel(LINEPlanetCall::AudioDeviceService audioDeviceService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService) {
        m_callService = callService;
        m_groupCallService = groupCallService;
        m_audioDeviceService = audioDeviceService;

        m_audioDeviceServiceToken = m_audioDeviceService.PropertyChanged({ this, &AudioDeviceViewModel::audioDevicePropertyChanged });
    }

    AudioDeviceViewModel::~AudioDeviceViewModel() {
        m_callService = nullptr;
        m_groupCallService = nullptr;

        m_audioDeviceService.PropertyChanged(m_audioDeviceServiceToken);
    }

    event_token AudioDeviceViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void AudioDeviceViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    CallType AudioDeviceViewModel::callType() {
        return m_callType;
    }

    void AudioDeviceViewModel::callType(CallType callType) {
        m_callType = callType;
    }

    Windows::Foundation::Collections::IObservableVector<AudioDevice> AudioDeviceViewModel::inputs() {
        return m_audioDeviceService.inputs();
    }

    Windows::Foundation::Collections::IObservableVector<AudioDevice> AudioDeviceViewModel::outputs() {
        return m_audioDeviceService.outputs();
    }

    int32_t AudioDeviceViewModel::selectedInputDeviceIndex() {
        return m_audioDeviceService.selectedInputDeviceIndex();
    }

    void AudioDeviceViewModel::selectedInputDeviceIndex(int32_t selectedIndex) {
        m_audioDeviceService.selectedInputDeviceIndex(selectedIndex);
    }

    int32_t AudioDeviceViewModel::selectedOutputDeviceIndex() {
        return m_audioDeviceService.selectedOutputDeviceIndex();
    }

    void AudioDeviceViewModel::selectedOutputDeviceIndex(int32_t selectedIndex) {
        m_audioDeviceService.selectedOutputDeviceIndex(selectedIndex);
    }

    hstring AudioDeviceViewModel::inputDeviceUid() {
        return m_audioDeviceService.inputDeviceUid();
    }

    hstring AudioDeviceViewModel::outputDeviceUid() {
        return m_audioDeviceService.outputDeviceUid();
    }
    
    void AudioDeviceViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void AudioDeviceViewModel::audioDevicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"inputDeviceChanged") {
            hstring inputDeviceUid = m_audioDeviceService.inputDeviceUid();
            if (m_callType == CallType::call && isCallActive(m_callService.callState())) {
                // change audio input device
                m_callService.audioInputDeviceChanged(inputDeviceUid);
            }
            else if (m_callType == CallType::groupCall) {
                m_groupCallService.audioInputDeviceChanged(inputDeviceUid);
            }
        }
        else if (e.PropertyName() == L"outputDeviceChanged") {
            hstring outputDeviceUid = m_audioDeviceService.outputDeviceUid();
            if (m_callType == CallType::call && isCallActive(m_callService.callState())) {
                // change audio output device
                m_callService.audioOutputDeviceChanged(outputDeviceUid);
            }
            else if (m_callType == CallType::groupCall && m_groupCallService.groupCallState() == ConnectState::connected) {
                m_groupCallService.audioOutputDeviceChanged(outputDeviceUid);
            }
        }
        else if (e.PropertyName() == L"defaultInputDeviceChanged") {
            if (m_audioDeviceService.selectedInputDeviceIndex() == 0) {
                // system default device is changed
                hstring inputDeviceUid = m_audioDeviceService.inputDeviceUid();

                if (m_callType == CallType::call && isCallActive(m_callService.callState())) {
                    m_callService.audioInputDeviceChanged(inputDeviceUid);
                }
                else if (m_callType == CallType::groupCall) {
                    m_groupCallService.audioInputDeviceChanged(inputDeviceUid);
                }
            }
        }
        else if (e.PropertyName() == L"defaultOutputDeviceChanged") {
            if (m_audioDeviceService.selectedOutputDeviceIndex() == 0) {
                // system default device is changed
                hstring outputDeviceUid = m_audioDeviceService.outputDeviceUid();

                if (m_callType == CallType::call && isCallActive(m_callService.callState())) {
                    m_callService.audioOutputDeviceChanged(outputDeviceUid);
                }
                else if (m_callType == CallType::groupCall && m_groupCallService.groupCallState() == ConnectState::connected) {
                    m_groupCallService.audioOutputDeviceChanged(outputDeviceUid);
                }
            }
        }
        else {
            dispatchPropertyChanged(e.PropertyName());
        }
    }

    bool AudioDeviceViewModel::isCallActive(ConnectState callState) {
        return (callState == ConnectState::connected || callState == ConnectState::waitAnswer || callState == ConnectState::verified) ? true : false;
    }
}
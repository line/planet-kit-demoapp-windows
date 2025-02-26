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

#include "AudioDeviceService.h"

#if __has_include("AudioDeviceService.g.cpp")
#include "AudioDeviceService.g.cpp"
#endif

#include "UiUtils.h"

namespace winrt::LINEPlanetCall::implementation {
    AudioDeviceService::AudioDeviceService() {
        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
        m_audioManager = PlanetKit::PlanetKitManager::GetInstance()->GetAudioManager();

        PlanetKit::AudioDeviceInfoArray audioDeviceInfos;

        m_audioManager->RegisterAudioDeviceEvent(&m_audioDeviceEventListener);

        m_audioManager->GetAudioInputDeviceList(&audioDeviceInfos);
        if (audioDeviceInfos.Size() > 0) {
            // append 'system default'
            AudioDevice device(PlanetKitAudioDeviceType::input, Constants::audioDeviceSystemDefaultUid, resourceLoader.GetString(L"lp_demoapp_common_systemsetting/text"));
            m_audioDeviceInput.Append(device);
        }
        for (size_t idx = 0; idx < audioDeviceInfos.Size(); ++idx) {
            AudioDevice device(UiUtils::audioDeviceType(audioDeviceInfos[idx]->GetDeviceType()), audioDeviceInfos[idx]->GetID().c_str(), audioDeviceInfos[idx]->GetName().c_str());
            m_audioDeviceInput.Append(device);
        }

        m_audioManager->GetAudioOutputDeviceList(&audioDeviceInfos);
        if (audioDeviceInfos.Size() > 0) {
            // append 'system default'
            AudioDevice device(PlanetKitAudioDeviceType::output, Constants::audioDeviceSystemDefaultUid, resourceLoader.GetString(L"lp_demoapp_common_systemsetting/text"));
            m_audioDeviceOutput.Append(device);
        }

        for (size_t idx = 0; idx < audioDeviceInfos.Size(); ++idx) {
            AudioDevice device(UiUtils::audioDeviceType(audioDeviceInfos[idx]->GetDeviceType()), audioDeviceInfos[idx]->GetID().c_str(), audioDeviceInfos[idx]->GetName().c_str());
            m_audioDeviceOutput.Append(device);
        }
    }

    event_token AudioDeviceService::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void AudioDeviceService::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    Windows::Foundation::Collections::IObservableVector<AudioDevice> AudioDeviceService::inputs() {
        return m_audioDeviceInput;
    }

    Windows::Foundation::Collections::IObservableVector<AudioDevice> AudioDeviceService::outputs() {
        return m_audioDeviceOutput;
    }

    int32_t AudioDeviceService::selectedInputDeviceIndex() {
        return m_selectedInputDevice;
    }

    void AudioDeviceService::selectedInputDeviceIndex(int32_t selectedIndex) {
        if (m_selectedInputDevice != selectedIndex && selectedIndex >= 0 && m_audioDeviceInput.Size() > 0 || selectedIndex == 0 && m_audioDeviceInput.Size() == 0) {
            m_selectedInputDevice = selectedIndex;

            dispatchPropertyChanged(L"selectedInputDeviceIndex");
            dispatchPropertyChanged(L"inputDeviceChanged");
        }
        else {
            m_selectedInputDevice = selectedIndex;
        }
    }

    int32_t AudioDeviceService::selectedOutputDeviceIndex() {
        return m_selectedOutputDevice;
    }

    void AudioDeviceService::selectedOutputDeviceIndex(int32_t selectedIndex) {
        if (m_selectedOutputDevice != selectedIndex && selectedIndex >= 0 && m_audioDeviceOutput.Size() > 0 || selectedIndex == 0 && m_audioDeviceOutput.Size() == 0) {
            m_selectedOutputDevice = selectedIndex;

            dispatchPropertyChanged(L"selectedOutputDeviceIndex");
            dispatchPropertyChanged(L"outputDeviceChanged");
        }
        else {
            m_selectedOutputDevice = selectedIndex;
        }
    }

    hstring AudioDeviceService::inputDeviceUid() {
        hstring result;
        if (m_audioDeviceInput.Size() > 0 && m_selectedInputDevice >= 0) {
            result = m_audioDeviceInput.GetAt(m_selectedInputDevice).uid();

            if (result == Constants::audioDeviceSystemDefaultUid) {
                // get system default audio device's uid
                PlanetKit::AudioDeviceInfoPtr inputDeviceInfo;
                m_audioManager->GetDefaultAudioInputDeviceInfo(&inputDeviceInfo);

                if (*inputDeviceInfo) {
                    result = inputDeviceInfo->GetID().c_str();
                }
                else {
                    result.clear();
                }
            }
        }

        return result;
    }

    hstring AudioDeviceService::outputDeviceUid() {
        hstring result;
        if (m_audioDeviceOutput.Size() > 0 && m_selectedOutputDevice >= 0) {
            result = m_audioDeviceOutput.GetAt(m_selectedOutputDevice).uid();

            if (result == Constants::audioDeviceSystemDefaultUid) {
                // get system default audio device's uid
                PlanetKit::AudioDeviceInfoPtr outputDeviceInfo;
                m_audioManager->GetDefaultAudioOutputDeviceInfo(&outputDeviceInfo);

                if (*outputDeviceInfo) {
                    result = outputDeviceInfo->GetID().c_str();
                }
                else {
                    result.clear();
                }
            }
        }

        return result;
    }

    void AudioDeviceService::defaultAudioDeviceChanged(PlanetKitAudioDeviceType type) {
        if (type == PlanetKitAudioDeviceType::input) {
            dispatchPropertyChanged(L"defaultInputDeviceChanged");
        }
        else if (type == PlanetKitAudioDeviceType::output) {
            dispatchPropertyChanged(L"defaultOutputDeviceChanged");
        }
    }

    void AudioDeviceService::addAudioDevice(PlanetKitAudioDeviceType type, hstring const& uid, hstring const& name) {
        AudioDevice device(type, uid, name);

        if (type == PlanetKitAudioDeviceType::input) {
            if (m_audioDeviceInput.Size() == 0) {
                Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
                AudioDevice defaultDevice(type, Constants::audioDeviceSystemDefaultUid, resourceLoader.GetString(L"lp_demoapp_common_systemsetting/text"));

                m_audioDeviceInput.Append(defaultDevice);
                m_audioDeviceInput.Append(device);

                selectedInputDeviceIndex(0);
            }
            else {
                m_audioDeviceInput.Append(device);
            }
            
        }
        else {
            if (m_audioDeviceOutput.Size() == 0) {
                Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;
                AudioDevice defaultDevice(type, Constants::audioDeviceSystemDefaultUid, resourceLoader.GetString(L"lp_demoapp_common_systemsetting/text"));

                m_audioDeviceOutput.Append(defaultDevice);
                m_audioDeviceOutput.Append(device);

                selectedOutputDeviceIndex(0);
            }
            else {
                m_audioDeviceOutput.Append(device);
            }
        }
    }

    void AudioDeviceService::removeAudioDevice(PlanetKitAudioDeviceType type, hstring const& uid) {
        Windows::Foundation::Collections::IObservableVector<AudioDevice>& devices = type == PlanetKitAudioDeviceType::input ? m_audioDeviceInput : m_audioDeviceOutput;
        int32_t& selectedIndex = type == PlanetKitAudioDeviceType::input ? m_selectedInputDevice : m_selectedOutputDevice;

        for (int32_t idx = 0; idx < (int32_t)devices.Size(); ++idx) {
            if (devices.GetAt(idx).uid() == uid) {
                if (selectedIndex == idx) {
                    selectedIndex = -1;
                }

                devices.RemoveAt(idx);

                if (type == PlanetKitAudioDeviceType::input && selectedIndex == -1) {
                    selectedInputDeviceIndex(0);
                }
                else if (type == PlanetKitAudioDeviceType::output && selectedIndex == -1) {
                    selectedOutputDeviceIndex(0);
                }

                break;
            }
        }

        if (devices.Size() == 1) {
            // It means there is no device to select
            devices.RemoveAt(0);
        }
    }
    
    void AudioDeviceService::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }
}
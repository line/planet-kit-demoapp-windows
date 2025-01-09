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

#include "CallService.g.h"

#include "PlanetKitManager.h"
#include "CallEventListener.h"

namespace winrt::LINEPlanetCall::implementation {
    struct CallService : CallServiceT<CallService> {
        CallService(LINEPlanetCall::MainWindow parentWindow);
        virtual ~CallService();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        PlanetKit::PlanetKitCallPtr planetKitCall();

        hstring peerId();
        void peerId(hstring const& peerId);

        ConnectState callState();
        void callState(ConnectState callState);

        bool peerMute();
        bool myMute();
        PlanetKitMediaType mediaType();

        bool myVideoEnabled();
        bool peerVideoEnabled();
        int32_t peerVolume();

        int32_t disconnectReason();

        hstring alertMessage();
        
        // methods
        Windows::Foundation::IAsyncOperation<ConnectResult> makeCall(hstring userId, hstring accessToken, bool videoCall, hstring audioInputDeviceUid, hstring audioOutputDeviceUid);
        bool verifyCall(hstring const& userId, hstring const& peerId, hstring const& ccParam, PlanetKitMediaType mediaType, hstring audioInputDeviceUid, hstring audioOutputDeviceUid);
        void endCall();
        void acceptCall();
        void muteMyAudio();

        void pauseMyVideo();
        void resumeMyVideo();

        void audioInputDeviceChanged(hstring const& uid);
        void audioOutputDeviceChanged(hstring const& uid);

        // call event methdos
        void onConnected();
        void onDisconnected(int32_t reason);

        void onPeerMute(bool mute);
        void onPeerVideoEnabled(bool enabled);
        void onPeerVolumeChanged(int32_t volume);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
        void onTitleTimer(IInspectable const& sender, IInspectable const& event);

    private:
        void clear();
        PlanetKit::AudioDevicePtr audioDevice(PlanetKitAudioDeviceType type, hstring const& uid);

    private:
        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };

        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        PlanetKit::PlanetKitCallPtr m_planetKitCall;

        CallEventListener m_callEventListener{ *this };

        PlanetKit::AudioDevicePtr m_input;
        PlanetKit::AudioDevicePtr m_output;

        hstring m_peerId;
        ConnectState m_callState = ConnectState::idle;

        bool m_peerMute = false;
        bool m_myMute = false;

        PlanetKitMediaType m_mediaType = PlanetKitMediaType::unknown;

        bool m_myVideoEnabled = true;
        bool m_peerVideoEnabled = false;
        int32_t m_peerVolume = 0;

        int32_t m_disconnectReason = 0;

        hstring m_alertMessage;

        Microsoft::UI::Xaml::DispatcherTimer m_callTitleTimer;
        event_token m_callTitleTimerToken;

        uint32_t m_callConnectedTime = 0;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct CallService : CallServiceT<CallService, implementation::CallService> {

    };
}
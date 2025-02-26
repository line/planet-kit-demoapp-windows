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

#include "GroupCallService.g.h"

#include "GroupCallEventListener.h"

namespace winrt::LINEPlanetCall::implementation {
    class ParticipantUpdatedEventListener {
    public:
        virtual void participantUpdated(PlanetKit::ConferencePeerUpdateParamPtr pParam) = 0;
    };

    struct GroupCallService : GroupCallServiceT<GroupCallService> {
        GroupCallService(LINEPlanetCall::MainWindow parentWindow);
        virtual ~GroupCallService();

        void registerParticipantUpdatedEvent(ParticipantUpdatedEventListener* eventListener);
        void unregisterParticipantUpdatedEvent();

        void onPeerListUpdated(PlanetKit::ConferencePeerUpdateParamPtr pParam);

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        hstring roomId();
        void roomId(hstring const& roomId);

        ConnectState groupCallState();

        bool myMute();
        bool myVideoEnabled();

        int32_t disconnectReason();

        hstring alertMessage();

        void clear();

        Windows::Foundation::IAsyncOperation<ConnectResult> joinConference(hstring userId, hstring userName, hstring accessToken, hstring audioInputDeviceUid, hstring audioOutputDeviceUid);
        void leaveConference();

        void muteMyAudio();
        void pauseMyVideo();
        void resumeMyVideo();

        void audioInputDeviceChanged(hstring const& uid);
        void audioOutputDeviceChanged(hstring const& uid);

        void onConnected();
        void onDisconnected(int32_t reason);

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);
        void onTitleTimer(IInspectable const& sender, IInspectable const& event);

    private:
        PlanetKit::AudioDevicePtr audioDevice(PlanetKitAudioDeviceType type, hstring const& uid);
    
    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        LINEPlanetCall::MainWindow m_parentWindow{ nullptr };

        bool m_myMute = false;
        bool m_myVideoEnabled = true;

        int32_t m_disconnectReason = 0;

        hstring m_alertMessage;

        hstring m_roomId;

        ConnectState m_groupCallState = ConnectState::idle;

        PlanetKit::AudioDevicePtr m_input;
        PlanetKit::AudioDevicePtr m_output;

        PlanetKit::PlanetKitConferencePtr m_planetKitConference;
        GroupCallEventListener m_groupCallEventListener{ *this };

        ParticipantUpdatedEventListener* m_participantUpdatedEventListener = nullptr;

        Microsoft::UI::Xaml::DispatcherTimer m_groupCallTitleTimer;
        event_token m_groupCallTitleTimerToken;

        uint32_t m_callConnectedTime = 0;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct GroupCallService : GroupCallServiceT<GroupCallService, implementation::GroupCallService> {

    };
}
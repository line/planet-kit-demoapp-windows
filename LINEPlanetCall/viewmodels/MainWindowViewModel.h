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

#include "MainWindowViewModel.g.h"

namespace winrt::LINEPlanetCall::implementation {
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel> {
        MainWindowViewModel(LINEPlanetCall::SettingService settingService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService);
        virtual ~MainWindowViewModel();

        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;

        AccountStatus accountStatus();

        double_t frameHeight();
        void frameHeight(double_t height);

        ConnectState callState();
        hstring callPeerId();

        PlanetKitMediaType mediaType();
        Microsoft::UI::Xaml::Visibility callVerified();

        Microsoft::UI::Xaml::Visibility incomingAudioCall();
        Microsoft::UI::Xaml::Visibility incomingVideoCall();

        ConnectState groupCallState();

        int32_t callDisconnectReason();

        hstring planetKitVersion();
        hstring appVersion();
        
        Windows::Foundation::IAsyncOperation<NotificationResult> notification();
        void stopNotification();

        bool verifyCall(hstring const& peerId, hstring const& ccParam, PlanetKitMediaType mediaType, hstring inputDeviceUid, hstring outputDeviceUid);
        void endCall();
        void acceptCall();

    protected:
        void dispatchPropertyChanged(hstring const& propertyName);

        void settingServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
        void callServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
        void groupCallServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

    private:
        void makeVersionString();
    
    private:
        const std::wstring VersionStringPrefix = L"{{major.minor.patch}}";

        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        double_t m_frameHeight = 500.0f;

        Microsoft::UI::Xaml::Visibility m_callVerified = Microsoft::UI::Xaml::Visibility::Collapsed;

        Microsoft::UI::Xaml::Visibility m_incomingAudioCall = Microsoft::UI::Xaml::Visibility::Collapsed;
        Microsoft::UI::Xaml::Visibility m_incomingVideoCall = Microsoft::UI::Xaml::Visibility::Collapsed;

        LINEPlanetCall::SettingService m_settingService{ nullptr };
        event_token m_tokenSettingService;

        LINEPlanetCall::CallService m_callService{ nullptr };
        event_token m_tokenCallService;

        LINEPlanetCall::GroupCallService m_groupCallService{ nullptr };
        event_token m_tokenGroupCallService;

        hstring m_planetKitVersion;
        hstring m_appVersion;
    };
};

namespace winrt::LINEPlanetCall::factory_implementation {
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, implementation::MainWindowViewModel> {

    };
}
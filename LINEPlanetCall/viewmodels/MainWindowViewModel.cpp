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

#include "MainWindowViewModel.h"

#include <winrt/Windows.Data.Json.h>

#if __has_include("MainWindowViewModel.g.cpp")
#include "MainWindowViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    MainWindowViewModel::MainWindowViewModel(LINEPlanetCall::SettingService settingService, LINEPlanetCall::CallService callService, LINEPlanetCall::GroupCallService groupCallService) {
        m_settingService = settingService;
        m_callService = callService;
        m_groupCallService = groupCallService;

        // make version string
        makeVersionString();

        m_tokenSettingService = m_settingService.PropertyChanged({ this, &MainWindowViewModel::settingServiceProperyChanged });
        m_tokenCallService = m_callService.PropertyChanged({ this, &MainWindowViewModel::callServiceProperyChanged });
        m_tokenGroupCallService = m_groupCallService.PropertyChanged({ this, &MainWindowViewModel::groupCallServiceProperyChanged });

        // load saved user info.
        m_settingService.loadUserInfo();
    }

    MainWindowViewModel::~MainWindowViewModel() {
        m_settingService.PropertyChanged(m_tokenSettingService);
        m_callService.PropertyChanged(m_tokenCallService);
        m_groupCallService.PropertyChanged(m_tokenGroupCallService);
    }

    event_token MainWindowViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void MainWindowViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    AccountStatus MainWindowViewModel::accountStatus() {
        return m_settingService.accountStatus();
    }

    double_t MainWindowViewModel::frameHeight() {
        return m_frameHeight;
    }

    void MainWindowViewModel::frameHeight(double_t height) {
        if (m_frameHeight != height) {
            m_frameHeight = height;

            dispatchPropertyChanged(L"frameHeight");
        }
    }

    ConnectState MainWindowViewModel::callState() {
        return m_callService.callState();
    }

    hstring MainWindowViewModel::callPeerId() {
        return m_callService.peerId();
    }

    PlanetKitMediaType MainWindowViewModel::mediaType() {
        return m_callService.mediaType();
    }

    Microsoft::UI::Xaml::Visibility MainWindowViewModel::callVerified() {
        return m_callVerified;
    }

    Microsoft::UI::Xaml::Visibility MainWindowViewModel::incomingAudioCall() {
        return m_incomingAudioCall;
    }

    Microsoft::UI::Xaml::Visibility MainWindowViewModel::incomingVideoCall() {
        return m_incomingVideoCall;
    }

    ConnectState MainWindowViewModel::groupCallState() {
        return m_groupCallService.groupCallState();
    }

    int32_t MainWindowViewModel::callDisconnectReason() {
        return m_callService.disconnectReason();
    }

    hstring MainWindowViewModel::planetKitVersion() {
        return m_planetKitVersion;
    }

    hstring MainWindowViewModel::appVersion() {
        return m_appVersion;
    }

    Windows::Foundation::IAsyncOperation<NotificationResult> MainWindowViewModel::notification() {
        co_return co_await m_settingService.notification();
    }

    void MainWindowViewModel::stopNotification() {
        m_settingService.stopNotification();
    }

    bool MainWindowViewModel::verifyCall(hstring const& peerId, hstring const& ccParam, PlanetKitMediaType mediaType, hstring inputDeviceUid, hstring outputDeviceUid) {
        bool result = m_callService.verifyCall(m_settingService.userId(), peerId, ccParam, mediaType, inputDeviceUid, outputDeviceUid);

        if (result == true) {
            m_callService.peerId(peerId);
            dispatchPropertyChanged(L"callPeerId");

            if (mediaType == PlanetKitMediaType::audioVideo) {
                m_incomingAudioCall = Microsoft::UI::Xaml::Visibility::Collapsed;
                m_incomingVideoCall = Microsoft::UI::Xaml::Visibility::Visible;
            }
            else {
                m_incomingAudioCall = Microsoft::UI::Xaml::Visibility::Visible;
                m_incomingVideoCall = Microsoft::UI::Xaml::Visibility::Collapsed;
            }

            dispatchPropertyChanged(L"incomingAudioCall");
            dispatchPropertyChanged(L"incomingVideoCall");
        }

        return result;
    }

    void MainWindowViewModel::endCall() {
        m_callService.endCall();
    }

    void MainWindowViewModel::acceptCall() {
        m_callService.acceptCall();
    }
    
    void MainWindowViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void MainWindowViewModel::settingServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"accountStatus") {
            dispatchPropertyChanged(e.PropertyName());
        }
    }

    void MainWindowViewModel::callServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"callState") {
            if (m_callService.callState() != ConnectState::verified) {
                m_callVerified = Microsoft::UI::Xaml::Visibility::Collapsed;
            }
            else {
                m_callVerified = Microsoft::UI::Xaml::Visibility::Visible;
            }

            dispatchPropertyChanged(L"callVerified");
            dispatchPropertyChanged(e.PropertyName());
        }
    }

    void MainWindowViewModel::groupCallServiceProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"groupCallState") {
            dispatchPropertyChanged(L"groupCallState");
        }
    }

    void MainWindowViewModel::makeVersionString() {
        Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resourceLoader;

        // app version
        // replace {{major.minor.patch}}
        std::wstring appVersion = resourceLoader.GetString(L"lp_demoapp_main_versioninfo1/Text").c_str();
        appVersion.replace(appVersion.find(VersionStringPrefix), VersionStringPrefix.size(), m_settingService.appVersion().c_str());
        m_appVersion = appVersion.c_str();

        // sdk version
        // relpace {{major.minor.patch}}
        std::wstring sdkVersion = resourceLoader.GetString(L"lp_demoapp_main_versioninfo2/Text").c_str();
        sdkVersion.replace(sdkVersion.find(VersionStringPrefix), VersionStringPrefix.size(), m_settingService.planetKitVersion().c_str());
        m_planetKitVersion = sdkVersion.c_str();
    }
}
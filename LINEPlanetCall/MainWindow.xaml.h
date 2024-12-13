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

#include "MainWindow.g.h"

namespace winrt::LINEPlanetCall::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        virtual ~MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();

        Microsoft::UI::Xaml::Controls::Frame mainFrame();

        LINEPlanetCall::SettingService settingService();
        LINEPlanetCall::CallService callService();
        LINEPlanetCall::AudioDeviceService audioDeviceService();
        LINEPlanetCall::CameraDeviceService cameraDeviceService();
        LINEPlanetCall::GroupCallService groupCallService();

        LINEPlanetCall::MainWindowViewModel mainWindowViewModel();

        void endCall(int32_t disconnectReason);
        void connectFailed(ConnectResult connectResult);

        void endCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void acceptCall(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void closedMainWindow(IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);
        void sizeChangedMainWindow(IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowSizeChangedEventArgs const& args);

    protected:
        void mainWindowViewModelProperyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

        void navigateMainPage();
        void navigateVerifiedVideoCallPage();
        Windows::Foundation::IAsyncAction showEndCall(int32_t disconnectReason);
        Windows::Foundation::IAsyncAction showConnectFailed(ConnectResult connectResult);

    private:
        void startNotification();
        void stopNotification();

        void navigateCall();

    private :
        LINEPlanetCall::SettingService m_settingService;
        LINEPlanetCall::CallService m_callService{ *this };
        LINEPlanetCall::AudioDeviceService m_audioDeviceService;
        LINEPlanetCall::CameraDeviceService m_cameraDeviceService;
        LINEPlanetCall::GroupCallService m_groupCallService{ *this };

        LINEPlanetCall::MainWindowViewModel m_mainWindowViewModel{ nullptr };
        event_token m_tokenMainWindowViewModel;
    };
}

namespace winrt::LINEPlanetCall::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}

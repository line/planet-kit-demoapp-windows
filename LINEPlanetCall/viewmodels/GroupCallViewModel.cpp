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

#include "GroupCallViewModel.h"

#if __has_include("GroupCallViewModel.g.cpp")
#include "GroupCallViewModel.g.cpp"
#endif

namespace winrt::LINEPlanetCall::implementation {
    GroupCallViewModel::GroupCallViewModel(LINEPlanetCall::GroupCallService groupCallService) {
        m_groupCallService = groupCallService;
        m_groupCallServiceToken = m_groupCallService.PropertyChanged({ this, &GroupCallViewModel::groupCallServicePropertyChanged });

        m_alertMessageShowTimer.Interval(std::chrono::milliseconds{ Constants::alertTimer });
        m_alertMessageShowTimerToken = m_alertMessageShowTimer.Tick({ this, &GroupCallViewModel::onTick });
    }

    GroupCallViewModel::~GroupCallViewModel() {
        m_alertMessageShowTimer.Tick(m_alertMessageShowTimerToken);
        m_groupCallService.PropertyChanged(m_groupCallServiceToken);
    }

    event_token GroupCallViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_propertyChanged.add(handler);
    }

    void GroupCallViewModel::PropertyChanged(event_token const& token) noexcept {
        m_propertyChanged.remove(token);
    }

    ConnectState GroupCallViewModel::groupCallState() {
        return m_groupCallService.groupCallState();
    }

    int32_t GroupCallViewModel::disconnectReason() {
        return m_groupCallService.disconnectReason();
    }

    bool GroupCallViewModel::myMute() {
        return m_groupCallService.myMute();
    }

    bool GroupCallViewModel::myVideoEnabled() {
        return m_groupCallService.myVideoEnabled();
    }

    Microsoft::UI::Xaml::Visibility GroupCallViewModel::myVideoPaused() {
        return m_groupCallService.myVideoEnabled() == true ? Microsoft::UI::Xaml::Visibility::Collapsed : Microsoft::UI::Xaml::Visibility::Visible;
    }

    Microsoft::UI::Xaml::Visibility GroupCallViewModel::visibleAlertMessage() {
        return m_visibleAlertMessage;
    }

    hstring GroupCallViewModel::alertMessage() {
        return m_groupCallService.alertMessage();
    }

    void GroupCallViewModel::leaveConference() {
        m_groupCallService.leaveConference();
    }

    void GroupCallViewModel::muteMyAudio() {
        m_groupCallService.muteMyAudio();
    }

    void GroupCallViewModel::pauseMyVideo() {
        m_groupCallService.pauseMyVideo();
    }

    void GroupCallViewModel::resumeMyVideo() {
        m_groupCallService.resumeMyVideo();
    }
    
    void GroupCallViewModel::dispatchPropertyChanged(hstring const& propertyName) {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    void GroupCallViewModel::onTick(IInspectable const& sender, IInspectable const& event) {
        std::ignore = sender;
        std::ignore = event;

        m_alertMessageShowTimer.Stop();
        m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Collapsed;

        dispatchPropertyChanged(L"visibleAlertMessage");
    }

    void GroupCallViewModel::groupCallServicePropertyChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e) {
        std::ignore = sender;

        if (e.PropertyName() == L"myVideoEnabled") {
            dispatchPropertyChanged(L"myVideoPaused");
            dispatchPropertyChanged(e.PropertyName());
        }
        else if (e.PropertyName() == L"alertMessage") {
            m_visibleAlertMessage = Microsoft::UI::Xaml::Visibility::Visible;
            m_alertMessageShowTimer.Start();

            dispatchPropertyChanged(L"alertMessage");
            dispatchPropertyChanged(L"visibleAlertMessage");
        }
        else {
            dispatchPropertyChanged(e.PropertyName());
        }
    }
}
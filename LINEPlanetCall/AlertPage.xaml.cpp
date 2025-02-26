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
#include "AlertPage.xaml.h"
#if __has_include("AlertPage.g.cpp")
#include "AlertPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LINEPlanetCall::implementation
{
    int32_t AlertPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void AlertPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void AlertPage::title(hstring const& title) {
        m_title = title;
    }

    hstring AlertPage::title() {
        return m_title;
    }

    void AlertPage::content(hstring const& content) {
        m_content = content;
    }

    hstring AlertPage::content() {
        return m_content;
    }

    void AlertPage::buttonString(hstring const& buttonString) {
        m_buttonString = buttonString;
    }

    hstring AlertPage::buttonString() {
        return m_buttonString;
    }

    Microsoft::UI::Xaml::Controls::ContentDialog AlertPage::dialog() {
        return m_dialog;
    }

    void AlertPage::dialog(Microsoft::UI::Xaml::Controls::ContentDialog const& dialog) {
        m_dialog = dialog;
    }

    void AlertPage::clickClose(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args) {
        std::ignore = sender;
        std::ignore = args;

        m_dialog.Hide();
    }
}

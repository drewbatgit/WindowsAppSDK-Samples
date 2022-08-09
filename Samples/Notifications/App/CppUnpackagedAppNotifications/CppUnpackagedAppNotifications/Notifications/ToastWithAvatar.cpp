﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"
#include "ToastWithAvatar.h"
#include "Common.h"
#include <winrt/Microsoft.Windows.AppNotifications.h>
#include <winrt/Microsoft.Windows.AppNotifications.Builder.h>
#include <winrt/Windows.Foundation.h>
#include "App.xaml.h"
#include "MainPage.xaml.h"

namespace winrt
{
    using namespace Microsoft::Windows::AppNotifications;
    using namespace Microsoft::Windows::AppNotifications::Builder;

    using namespace CppUnpackagedAppNotifications::implementation;
}

const wchar_t* ToastWithAvatar::ScenarioName{ L"Local Toast with Avatar Image" };

bool ToastWithAvatar::SendToast()
{
    // The ScenarioIdToken uniquely identify a scenario and is used to route the response received when the user clicks on a toast to the correct scenario.
    //auto ScenarioIdToken{ Common::MakeScenarioIdToken(ToastWithAvatar::ScenarioId) };
#if 0
    winrt::hstring xmlPayload{
        L"<toast launch = \"action=ToastClick&amp;" + ScenarioIdToken + L"\">\
            <visual>\
                <binding template = \"ToastGeneric\">\
                    <image placement = \"appLogoOverride\" hint-crop=\"circle\" src = \"" + winrt::App::GetFullPathToAsset(L"Square150x150Logo.png") + L"\"/>\
                    <text>" + ScenarioName + L"</text>\
                    <text>This is an example message using XML</text>\
                </binding>\
            </visual>\
            <actions>\
                <action\
                    content = \"Open App\"\
                    arguments = \"action=OpenApp&amp;" + ScenarioIdToken + L"\"/>\
            </actions>\
        </toast>" };

    auto toast{ winrt::AppNotification(xmlPayload) };
#endif
    winrt::Microsoft::Windows::AppNotifications::Builder::AppNotificationBuilder();
    auto appNotification{ winrt::AppNotification(winrt::AppNotificationBuilder()
        .AddArgument(L"action", L"ToastClick")
        .AddArgument(L"scenarioId", std::to_wstring(ToastWithAvatar::ScenarioId))
        .SetAppLogoOverride(winrt::Windows::Foundation::Uri(L"file://" + winrt::App::GetFullPathToAsset(L"Square150x150Logo.png")), winrt::AppNotificationImageCrop::Circle)
        .AddText(ScenarioName)
        .AddText(L"This is an example message using XML")
        .AddButton(winrt::AppNotificationButton(L"Open App")
            .AddArgument(L"action", L"OpenApp")
            .AddArgument(L"scenarioId", std::to_wstring(ToastWithAvatar::ScenarioId)))
        .GetXmlPayload()) };

    winrt::AppNotificationManager::Default().Show(appNotification);

    return appNotification.Id() != 0; // return true (indicating success) if the toast was sent (if it has an Id)
}

void ToastWithAvatar::NotificationReceived(winrt::Microsoft::Windows::AppNotifications::AppNotificationActivatedEventArgs const& notificationActivatedEventArgs)
{
    winrt::CppUnpackagedAppNotifications::Notification notification{};
    notification.Originator = ScenarioName;
    auto action{ Common::ExtractParamFromArgs(notificationActivatedEventArgs.Argument().c_str(), L"action") };
    notification.Action = action.has_value() ? action.value() : L"";
    winrt::MainPage::Current().NotificationReceived(notification);
    winrt::App::ToForeground();
}

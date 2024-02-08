#include "pch.h"
#include "IWindow.h"

IWindow::IWindow(const string& windowTitle)
    : title(windowTitle)
    , displayTitle(title)
{
    if (!titles.insert({windowTitle, 0}).second)
    {
        title += "##" + to_string(titles[windowTitle]++);
    }
}

const string_view& IWindow::GetTitle() const
{
    return displayTitle;
}

const char* IWindow::GetImGuiTitle() const
{
    return title.c_str();
}

void IWindow::ImGuiDrawBegin()
{
    ImGui::Begin(GetImGuiTitle(), isClosable ? &isClosed : nullptr);
    windowPosition = ImGui::GetWindowPos();
    windowSize = ImGui::GetWindowSize();
}

void IWindow::ImGuiDrawEnd()
{
    ImGui::End();
}

void IWindow::DrawDebugMenuItems()
{
    ImGui::Text("Window Position: %i, %i", windowPosition.x, windowPosition.y);
    ImGui::Text("Window Size: %i, %i", windowSize.x, windowSize.y);
}

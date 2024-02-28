#include "pch.h"
#include "IWindow.h"

IWindow::IWindow(const string& windowTitle)
    : mTtitle(windowTitle)
    , mDisplayTitle(mTtitle)
{
    if (!sTitles.insert({windowTitle, 0}).second)
    {
        mTtitle += "##" + to_string(sTitles[windowTitle]++);
    }
}

const string_view& IWindow::GetTitle() const
{
    return mDisplayTitle;
}

const char* IWindow::GetImGuiTitle() const
{
    return mTtitle.c_str();
}

void IWindow::ImGuiDrawBegin()
{
    if (mHasMenuBar)
        ImGui::Begin(GetImGuiTitle(), mIsClosable ? &mIsClosed : nullptr, ImGuiWindowFlags_MenuBar);
    else
        ImGui::Begin(GetImGuiTitle(), mIsClosable ? &mIsClosed : nullptr);

    mWindowPosition = ImGui::GetWindowPos();
    mWindowSize = ImGui::GetWindowSize();
}

void IWindow::ImGuiDrawEnd()
{
    ImGui::End();
}

void IWindow::DrawDebugMenuItems()
{
    ImGui::Text("Window Position: %i, %i", mWindowPosition.x, mWindowPosition.y);
    ImGui::Text("Window Size: %i, %i", mWindowSize.x, mWindowSize.y);
}

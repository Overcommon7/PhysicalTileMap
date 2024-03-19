#include "pch.h"
#include "IWindow.h"

#include "App/ILayer.h"

IWindow::IWindow(const string& windowTitle)
    : mTitle(windowTitle)
    , mDisplayTitle(mTitle)
{
    if (!sTitles.insert({windowTitle, 0}).second)
    {
        mTitle += "##" + to_string(sTitles[windowTitle]++);
    }
}

const string_view& IWindow::GetTitle() const
{
    return mDisplayTitle;
}

const char* IWindow::GetImGuiTitle() const
{
    return mTitle.c_str();
}

void IWindow::ImGuiDrawBegin()
{
    if (mIsPopup)
    {
        if (!mIsOpen)
            return;

        ImGui::OpenPopup(mTitle.c_str());
        mPopOpenSuccessful = ImGui::BeginPopupModal(mTitle.c_str(), &mIsOpen, ImGuiWindowFlags_AlwaysAutoResize);
    }
    else
    {
        if (mHasMenuBar)
            ImGui::Begin(GetImGuiTitle(), mIsClosable ? &mIsOpen : nullptr, ImGuiWindowFlags_MenuBar);
        else
            ImGui::Begin(GetImGuiTitle(), mIsClosable ? &mIsOpen : nullptr);

        mWindowPosition = ImGui::GetWindowPos();
        mWindowSize = ImGui::GetWindowSize();
    }



}

void IWindow::ImGuiDrawEnd()
{
    if (mIsPopup)
    {
        if (mPopOpenSuccessful)
        {
            ImGui::EndPopup();
        }          
    }
    else
    {
        ImGui::End();
    }
    
}

void IWindow::DrawDebugMenuItems()
{
    ImGui::Text("Window Position: %i, %i", mWindowPosition.x, mWindowPosition.y);
    ImGui::Text("Window Size: %i, %i", mWindowSize.x, mWindowSize.y);
}

void IWindow::Open()
{
    mIsOpen = true;
    if (mIsPopup)
        ILayer::SetCurrentPopup(this);
}

void IWindow::Close()
{
    mIsOpen = false;
    if (mIsPopup)
        ILayer::SetCurrentPopup(nullptr);
}

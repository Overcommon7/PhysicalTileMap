#include "pch.h"
#include "SaveAsModal.h"

#include "ImGuiUtilities/ImGuiUtils.h"

SaveAsModal::SaveAsModal()
	: IWindow("Save As")
	, mShouldSave(false)
{
	mIsClosable = true;
	mIsClosed = true;
}

void SaveAsModal::ImGuiDrawBegin()
{
	ImGui::BeginPopup(mTtitle.c_str());
}

void SaveAsModal::ImGuiDrawEnd()
{
	ImGui::EndPopup();
}

void SaveAsModal::ImGuiDraw()
{
	if (mIsClosed)
		return;

	if (!ImGui::BeginPopupModal(mTtitle.c_str(), &mIsClosed))
		return;

	ImGuiUtils::SerializeString("Folder", mSavePath);

	ImGui::SameLine();
	if (fs::exists(mSavePath) && fs::is_directory(mSavePath))
	{
		if (ImGui::Button("Save"))
		{
			mShouldSave = true;
			mIsClosed = true;
			ImGui::CloseCurrentPopup();
		}
	}
	else
	{
		ImGui::TextDisabled("Save");
	}

	if (ImGui::Button("Cancel"))
	{
		mShouldSave = false;
		mIsClosed = true;
	}

}

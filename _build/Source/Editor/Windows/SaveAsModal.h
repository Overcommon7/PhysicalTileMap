#pragma once
#include "ImGuiWindow/IWindow.h"

class SaveAsModal final : public IWindow    
{
public:
	SaveAsModal();

	void ImGuiDrawBegin() override;
	void ImGuiDrawEnd() override;
	void ImGuiDraw() override;

	bool ShouldSave() const { return mShouldSave; }
	void Saved() { mShouldSave = false; }
	fs::path GetSavePath() const { return fs::path(mSavePath); }
private:
	string mSavePath;
	bool mShouldSave;
};


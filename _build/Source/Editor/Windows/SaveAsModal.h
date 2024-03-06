#pragma once
#include "ImGuiWindow/IWindow.h"

class SaveAsModal final : public IWindow    
{
public:
	SaveAsModal();

	void ImGuiDrawBegin() override;
	void ImGuiDrawEnd() override;
	void ImGuiDraw() override;

	void Open() { mIsClosed = false; }
	void Close() { mIsClosed = true; }

	bool ShouldSave() const { return mShouldSave; }
	fs::path GetSavePath() const { return fs::path(mSavePath); }
private:
	string mSavePath;
	bool mShouldSave;
};


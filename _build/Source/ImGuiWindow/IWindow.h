#pragma once
class IWindow
{
public:
	virtual ~IWindow() {}
	const string_view& GetTitle() const;
	const char* GetImGuiTitle() const;

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void ImGuiDrawBegin();
	virtual void ImGuiDrawEnd();

	virtual void Update() {}
	virtual void ImGuiDraw() = 0;
	virtual void DrawDebugMenuItems();

	virtual void Open();
	virtual void Close();
	virtual void SetClosable(bool closable) { mIsClosable = closable; }

	bool IsOpen() const { return mIsOpen; }
	bool IsClosable() const { return mIsClosable; }
protected:
	string mTitle;
	string_view mDisplayTitle;
	bool mIsClosable = false;
	bool mIsOpen = true;
	bool mHasMenuBar = false;
	bool mIsPopup = false;
	Vector2Int mWindowPosition;
	Vector2Int mWindowSize;

	IWindow(const string& windowTitle);
private:
	inline static std::map<string, int> sTitles = {};
	bool mPopOpenSuccessful = false;
};

#define IWindowConstructor(className) className(const string& windowTitle) : IWindow(windowTitle)




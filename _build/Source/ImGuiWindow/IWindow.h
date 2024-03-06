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

	bool IsClosed() const { return mIsClosed; }
protected:
	string mTtitle;
	string_view mDisplayTitle;
	bool mIsClosable = false;
	bool mIsClosed = false;
	bool mHasMenuBar = false;
	Vector2Int mWindowPosition;
	Vector2Int mWindowSize;

	IWindow(const string& windowTitle);
private:
	inline static std::map<string, int> sTitles = {};
};

#define IWindowConstructor(className) className(const string& windowTitle) : IWindow(windowTitle)




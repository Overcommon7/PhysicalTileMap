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
protected:
	string title;
	string_view displayTitle;
	bool isClosable = false;
	bool isClosed = false;
	Vector2Int windowPosition;
	Vector2Int windowSize;

	IWindow(const string& windowTitle);
private:
	inline static std::map<string, int> titles = {};
};

#define IWindowConstructor(className) inline ##className(const string& windowTitle) : IWindow(windowTitle) {}




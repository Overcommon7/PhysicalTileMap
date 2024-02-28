#pragma once


class ImGuiUtils
{
private:
	inline static string label = "";
	inline static unordered_map<string, int> lables = {};
		
	static bool SerializeStringMulti(string_view name, string& str);
	static bool SerializeStringSingle(string_view name, string& str);
	static void NewFrame();

	friend class App;
public:
	
	static const char* GetLabel(string_view name);
	static bool SerializeInt(string_view name, int& value);
	static bool SerializeInt2(string_view name, Vector2Int& value);
	static bool SerializeVector2(string_view name, Vector2& vector, float speed = 0.5f);
	static bool SerializeFloat(string_view name, float& num, float speed = 0.5f);
	static bool SerializeString(string_view name, string& str, bool multiLine = false);
	static bool SerializeBool(string_view name, bool& value);
	static bool SerializeColor(string_view name, Color& color);
	static bool AddButtonMethod(string_view name, function<void()> func);

	static void DrawVector2(string_view name, Vector2 vec);
	static void DrawVector2Int(string_view name, Vector2Int vec);
	static void DrawString(string_view name, const string& str);
	static void DrawBool(string_view name, bool state);
	static void DrawInt(string_view name, int value);
	static void DrawFloat(string_view name, float value);

	constexpr static int MAX_SINGLE_LINE_STRING_LENGTH = 256;
	constexpr static int MAX_MULTI_LINE_STRING_LENGTH = MAX_SINGLE_LINE_STRING_LENGTH * 4;
};




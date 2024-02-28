#include "pch.h"
#include "ImGuiUtils.h"

struct InputTextCallback_UserData
{
	std::string*            Str;
	ImGuiInputTextCallback  ChainCallback;
	void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}

const char* ImGuiUtils::GetLabel(string_view name)
{
	label = name.data();
	if (!lables.insert({ label, 0 }).second)
	{
		++lables[label];
	}

	int number = lables[label];
	label += "##" + to_string(number);
	return label.c_str();
}

bool ImGuiUtils::SerializeStringMulti(string_view name, string& str)
{	
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = &str;
	cb_user_data.ChainCallback = InputTextCallback;
	cb_user_data.ChainCallbackUserData = nullptr;
	return ImGui::InputTextMultiline(GetLabel(name), str.data(), str.capacity() + 1, {0, 0}, flags, InputTextCallback, &cb_user_data);
}

bool ImGuiUtils::SerializeStringSingle(string_view name, string& str)
{
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = &str;
	cb_user_data.ChainCallback = InputTextCallback;
	cb_user_data.ChainCallbackUserData = nullptr;
	return ImGui::InputText(GetLabel(name), str.data(), str.capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

void ImGuiUtils::NewFrame()
{
	lables.clear();
}

bool ImGuiUtils::SerializeInt(string_view name, int& value)
{
	return ImGui::DragInt(GetLabel(name), &value);
}

bool ImGuiUtils::SerializeInt2(string_view name, Vector2Int& value)
{
	return ImGui::DragInt2(GetLabel(name), &value.x);
}

bool ImGuiUtils::SerializeVector2(string_view name, Vector2& vector, float speed)
{
	return ImGui::DragFloat2(GetLabel(name), &vector.x, speed);
}

bool ImGuiUtils::SerializeFloat(string_view name, float& num, float speed)
{
	return ImGui::DragFloat(GetLabel(name), &num, speed);
}

bool ImGuiUtils::SerializeString(string_view name, string& str, bool multiLine)
{
	return multiLine ? SerializeStringMulti(name, str) : SerializeStringSingle(name, str);
}

bool ImGuiUtils::SerializeBool(string_view name, bool& value)
{
	return ImGui::Checkbox(GetLabel(name), &value);
}

bool ImGuiUtils::SerializeColor(string_view name, Color& color)
{
	auto imguiColor = Convert(color);
	bool ret = ImGui::ColorEdit4(GetLabel(name), &imguiColor.x);
	if (ret)
	{
		color = Convert(imguiColor);
	}
	return ret;
}

bool ImGuiUtils::AddButtonMethod(string_view name, function<void()> func)
{
	if (ImGui::Button(GetLabel(name)))
	{
		func();
		return true;
	}

	return false;
}

void ImGuiUtils::DrawVector2(string_view name, Vector2 vec)
{
	ImGui::Text("%s: %f, %f", name.data(), vec.x, vec.y);
}

void ImGuiUtils::DrawVector2Int(string_view name, Vector2Int vec)
{
	ImGui::Text("%s: %i, %i", name.data(), vec.x, vec.y);
}

void ImGuiUtils::DrawString(string_view name, const string& str)
{
	ImGui::Text("%s: %s", name.data(), str.c_str());
}

void ImGuiUtils::DrawBool(string_view name, bool state)
{
	ImGui::Text("%s: %s", name.data(), state ? "true" : "false");
}

void ImGuiUtils::DrawInt(string_view name, int value)
{
	ImGui::Text("%s: %i", name.data(), value);
}

void ImGuiUtils::DrawFloat(string_view name, float value)
{
	ImGui::Text("%s: %f", name.data(), value);
}
								
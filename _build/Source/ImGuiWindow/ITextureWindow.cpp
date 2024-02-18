#include "pch.h"
#include "ITextureWindow.h"

void ITextureWindow::Terminate()
{
	camera.Terminate();
}

void ITextureWindow::BeginDraw()
{
	camera.BeginDrawing();
	RaylibDraw();
}

void ITextureWindow::EndDraw()
{
	camera.EndDrawing();
}

void ITextureWindow::ImGuiDraw()
{	
	DrawTextureToWindow();
	texturePosition = ImGui::GetItemRectMin();
	localMousePosition = GetTexturePoint(::GetMousePosition());	
}

void ITextureWindow::DrawDebugMenuItems()
{
	Vector2Int mousePosition(::GetMousePosition());
	ImGui::Text("Mouse Position: %i, %i", mousePosition.x, mousePosition.y);
	ImGui::Text("Local Mouse Position: %i, %i", localMousePosition.x, localMousePosition.y);
	ImGui::Text("Is Within Texture: %s", IsInsideTexture(mousePosition) ? "true" : "false");
}

Vector2Int ITextureWindow::GetMousePosition() const
{
	return localMousePosition;
}

Vector2Int ITextureWindow::GetTexturePoint(Vector2Int point) const
{	
	Vector2Int position = point - texturePosition;
	Vector2 ratio = Vector2Divide(camera.GetResolution(), textureSize);
	Vector2 cameraPosition(Vector2Multiply(ratio, position));
	return camera.TransformPoint(cameraPosition);
}

bool ITextureWindow::IsInsideTexture(Vector2Int point) const
{
	Rectangle rectangle(texturePosition.x, texturePosition.y, textureSize.x, textureSize.y);
	return CheckCollisionPointRec(point, rectangle);
}

ITextureWindow::ITextureWindow(const string& title, Vector2Int cameraResolution)
	: IWindow(title)
	, camera({0, 0}, cameraResolution)
	, localMousePosition({})
{
	camera.SetActive(false);
	camera.Center(false);
}

void ITextureWindow::DrawTextureToWindow()
{
	const RenderTexture& image = camera.GetRenderTexture();
	ImVec2 area = ImGui::GetContentRegionAvail();

	float scale =  area.x / image.texture.width;

	float y = image.texture.height * scale;
	if (y > area.y)
	{
		scale = area.y / image.texture.height;
	}

	textureSize.x = int(image.texture.width * scale);
	textureSize.y = int(image.texture.height * scale);


	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosX(area.x/2 - textureSize.x/2);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (area.y / 2 - textureSize.y / 2));


	rlImGuiImageRect(&image.texture, textureSize.x, textureSize.y, Rectangle{ 0,0, float(image.texture.width), -float(image.texture.height) });
}

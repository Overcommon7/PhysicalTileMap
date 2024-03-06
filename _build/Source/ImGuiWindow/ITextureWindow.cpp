#include "pch.h"
#include "ITextureWindow.h"

void ITextureWindow::Terminate()
{
	mCamera.Terminate();
}

void ITextureWindow::Update()
{
	mCamera.Update();
}

void ITextureWindow::BeginDraw()
{
	mCamera.BeginDrawing();
	RaylibDraw();
}

void ITextureWindow::EndDraw()
{
	mCamera.EndDrawing();
}

void ITextureWindow::ImGuiDraw()
{	
	DrawTextureToWindow();
	mTexturePosition = ImGui::GetItemRectMin();
	mLocalMousePosition = GetTexturePoint(::GetMousePosition());	
}

void ITextureWindow::DrawDebugMenuItems()
{
	Vector2Int mousePosition(::GetMousePosition());
	ImGui::Text("Mouse Position: %i, %i", mousePosition.x, mousePosition.y);
	ImGui::Text("Local Mouse Position: %i, %i", mLocalMousePosition.x, mLocalMousePosition.y);
	ImGui::Text("Is Within Texture: %s", IsInsideTexture(mousePosition) ? "true" : "false");
}

Vector2Int ITextureWindow::GetMousePosition() const
{
	return mLocalMousePosition;
}

Vector2Int ITextureWindow::GetTexturePoint(Vector2Int point) const
{	
	Vector2Int position = point - mTexturePosition;
	Vector2 ratio = Vector2Divide(mCamera.GetResolution(), mTextureSize);
	Vector2 cameraPosition(Vector2Multiply(ratio, position));
	return mCamera.TransformPoint(cameraPosition);
}

bool ITextureWindow::IsInsideTexture(Vector2Int point) const
{
	Rectangle rectangle(mTexturePosition.x, mTexturePosition.y, mTextureSize.x, mTextureSize.y);
	return CheckCollisionPointRec(point, rectangle);
}

ITextureWindow::ITextureWindow(const string& title, Vector2Int cameraResolution)
	: IWindow(title)
	, mCamera({0, 0}, cameraResolution)
	, mLocalMousePosition({})
{
	mCamera.SetActive(false);
	mCamera.Center(false);
}

void ITextureWindow::DrawTextureToWindow()
{
	const RenderTexture& image = mCamera.GetRenderTexture();
	ImVec2 area = ImGui::GetContentRegionAvail();

	float scale =  area.x / image.texture.width;

	float y = image.texture.height * scale;
	if (y > area.y)
	{
		scale = area.y / image.texture.height;
	}

	mTextureSize.x = int(image.texture.width * scale);
	mTextureSize.y = int(image.texture.height * scale);


	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosX(area.x/2 - mTextureSize.x/2);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (area.y / 2 - mTextureSize.y / 2));


	rlImGuiImageRect(&image.texture, mTextureSize.x, mTextureSize.y, Rectangle{ 0,0, float(image.texture.width), -float(image.texture.height) });
}

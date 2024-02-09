#include "pch.h"
#include "Camera.h"

namespace Render
{
	void Camera::BeginDrawing()
	{
		BeginTextureMode(renderTexture);
		ClearBackground(clearColor);
		BeginMode2D(camera);
	}

	void Camera::EndDrawing()
	{
		EndMode2D();
		EndTextureMode();
	}

	void Camera::SetClearColor(Color color)
	{
		clearColor = color;
	}

	void Camera::SetTintColor(Color color)
	{
		tintColor = color;
	}

	void Camera::Center(bool center)
	{
		this->center = center;
	}

	void Camera::SetRenderPosition(Vector2 relativePosition)
	{
		this->relativePosition = Vector2Clamp(relativePosition, Vector2Zero(), Vector2One());

		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();

		renderPosition.x = this->relativePosition.x * screenWidth;
		renderPosition.y = this->relativePosition.y * screenHeight;	
	}

	Vector2Int Camera::GetMousePosition()
	{
		if (isMousePositionCached)
			return cachedMousePosition;

		cachedMousePosition = TransformPoint(::GetMousePosition());

		isMousePositionCached = true;
		return cachedMousePosition;
	}

	void Camera::SetResolution(Vector2Int resolution)
	{
		if (resolution == GetResolution()) return;
		UnloadRenderTexture(renderTexture);
		renderTexture = LoadRenderTexture(resolution.x, resolution.y);
		renderSize = resolution;
		aspectRatio =  renderSize.x / renderSize.y;
	}

	Camera::Camera(Vector2 renderPosition, Vector2 virtualScreenSize)
		: camera({})
		, renderPosition({})
		, clearColor(BLACK)
		, tintColor(WHITE)
		, terminated(false)
		, center(renderPosition.x < 0)
		, usingVirtualScreenSize(true)
		, id(ID++)
		, isMousePositionCached(false)
		, isActive(true)
	{
		if (!IsWindowReady())
			throw std::exception("Window Must Be Created Before Camera Can Be Loaded");

		Vector2 screenSize(GetScreenWidth(), GetScreenHeight());
		if (virtualScreenSize.x < 0)
		{
			virtualScreenSize = screenSize;
			usingVirtualScreenSize = false;
		}
		else
		{
			virtualScreenSpaceRatio = Vector2Divide(virtualScreenSize, screenSize);
		}

		renderSize = virtualScreenSize;

		camera.zoom = 1;
		camera.offset = { };
		renderTexture = LoadRenderTexture((int)renderSize.x, (int)renderSize.y);
		aspectRatio = renderSize.x / renderSize.y;

		if (center)
			CenterRenderPosition();
		else
		{
			auto ratio = Vector2Divide(renderPosition, screenSize); 
			SetRenderPosition(ratio);
		}
		
		cameras.push_back(this);
	}

	Camera::~Camera()
	{
		if (terminated) return;

		Terminate();
	}

	void Camera::Terminate()
	{
		UnloadRenderTexture(renderTexture);
		terminated = true;
		cameras.erase(std::find(cameras.begin(), cameras.end(), this));
	}

	void Camera::Update()
	{
		isMousePositionCached = false;

		if (!IsWindowResized())	return;
		int screenWidth = 0;

		if (usingVirtualScreenSize)
		{
			renderSize.y = virtualScreenSpaceRatio.y * GetScreenHeight();
			screenWidth = virtualScreenSpaceRatio.x * GetScreenWidth();
		}
		else
		{
			renderSize.y = (float)GetScreenHeight();
			screenWidth = GetScreenWidth();
		}
		 
		renderSize.x = aspectRatio * renderSize.y;

		if (renderSize.x > screenWidth)
		{
			renderSize.y = aspectRatio * screenWidth;
			renderSize.x = screenWidth;
		}

		if (center)
			CenterRenderPosition();
		else
			SetRenderPosition(relativePosition);

		
	}

	void Camera::DrawFrameBuffer()
	{
		for (auto camera : cameras)
		{
			if (!camera->isActive) 
				continue;

			DrawTexturePro(camera->renderTexture.texture,
				{ 0, 0, (float)camera->renderTexture.texture.width, -(float)camera->renderTexture.texture.height },
				{ camera->renderPosition.x, camera->renderPosition.y, camera->renderSize.x, camera->renderSize.y },
				{ 0, 0 }, 0, camera->tintColor);
		}
	}

	Vector2Int Camera::GetMousePosition(int cameraID)
	{
		auto it = std::find_if(cameras.begin(), cameras.end(), [&cameraID](const Camera* camera) {
			return camera->id == cameraID;
			});

		if (it == cameras.end())
			return ::GetMousePosition();

		return (*it)->GetMousePosition();
	}

	bool Camera::IsMouseWithinBounds(int cameraID)
	{
		auto it = std::find_if(cameras.begin(), cameras.end(), [&cameraID](const Camera* camera) {
			return camera->id == cameraID;
			});

		if (it == cameras.end())
			return false;

		return (*it)->IsMouseWithinBounds();
	}

	vector<int> Camera::GetAllCameraIDs()
	{
		vector<int> ids;
		ids.reserve(cameras.size());

		for (auto camera : cameras)
			ids.push_back(camera->id);

		return ids;
	}

	bool Camera::IsMouseWithinBounds()
	{
		Rectangle rectangle(renderPosition.x, renderPosition.y, renderSize.x, renderSize.y);
		return CheckCollisionPointRec(::GetMousePosition(), rectangle);
	}

	Vector2Int Camera::TransformPoint(Vector2Int position)
	{
		Vector2 point = Vector2Subtract(position, renderPosition);
		point = GetScreenToWorld2D(point, camera);

		Vector2 pixelSize(renderTexture.texture.width, renderTexture.texture.height);
		Vector2 ratio = Vector2Divide(pixelSize, renderSize);

		return Vector2Multiply(point, ratio);
	}

	Camera& Camera::GetCameraFromID(int id)
	{
		auto it = std::find_if(cameras.begin(), cameras.end(), [&id](const Camera* camera) {
			return camera->id == id;
			});

		if (it == cameras.end())
			throw std::exception("Camera Does Not Exist");

		return *(*it);
	}

	void Camera::CenterRenderPosition()
	{
		int screenWidth = GetScreenWidth();
		int screenHeight = GetScreenHeight();

		if (renderSize.x >= screenWidth)
		{
			renderPosition.x = 0;
		}
		else
		{
			renderPosition.x = (screenWidth - renderSize.x) * 0.5f;
		}

		if (renderSize.y >= screenHeight)
		{
			renderPosition.y = 0;
		}
		else
		{
			renderPosition.y = (screenHeight - renderSize.y) * 0.5f;
		}	
	}
}

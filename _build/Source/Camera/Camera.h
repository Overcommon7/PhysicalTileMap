#pragma once
namespace Render
{
	class Camera
	{
	public:
		void BeginDrawing();
		void EndDrawing();

		void SetClearColor(Color color);
		void SetTintColor(Color color);
		void Center(bool center);

		void SetRenderPosition(Vector2 relativePosition);
		Vector2Int GetMousePosition();
		Vector2Int GetResolution() const { return Vector2(renderTexture.texture.width, renderTexture.texture.height); }
		Vector2Int GetRenderSize() const { return renderSize; }
		void SetResolution(Vector2Int resolution);

		Camera(Vector2 renderPosition = {-1, -1}, Vector2 virtualScreenSize = {-1, -1});
		~Camera();
		void Terminate();
		void Update();

		static void DrawFrameBuffer();
		static Vector2Int GetMousePosition(int cameraID);
		static bool IsMouseWithinBounds(int cameraID);
		static vector<int> GetAllCameraIDs();

		void UpdateZoom(float zoom) { camera.zoom += zoom; }
		void SetZoom(float zoom) { camera.zoom = zoom; }
		bool IsMouseWithinBounds();

		Vector2Int TransformPoint(Vector2Int position) const;
		void SetActive(bool active) { isActive = active; }

		const int GetID() const { return id; }
		
		static Camera& GetCameraFromID(int id);
		Camera2D& GetRaylibCamera() { return camera; }
		const RenderTexture& GetRenderTexture() const { return renderTexture; }
	private:
		Camera2D camera;
		RenderTexture renderTexture;
		Color clearColor;
		Color tintColor;
		Vector2 renderSize;
		Vector2 renderPosition;
		Vector2 relativePosition;
		Vector2 virtualScreenSpaceRatio;
		Vector2Int cachedMousePosition;
		float aspectRatio;
		bool terminated;
		bool center;
		bool usingVirtualScreenSize;
		int id;
		bool isMousePositionCached;
		bool isActive;

		static inline vector<Render::Camera*> cameras = {};

		void CenterRenderPosition();

		static inline int ID = 0;
	};
}



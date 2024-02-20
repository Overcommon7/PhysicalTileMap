#pragma once

class Sprite
{
public:
	enum class Type
	{
		Player,
		Platform,
		Other
	};
public:

	void SetTexture(Texture2D texture) { mTexture = texture; }
	void SetPosition(Vector2 position) { mPosition = position; }
	void SetPositionX(float position) { mPosition.x = position; }
	void SetPositionY(float position) { mPosition.y = position; }

	void Translate(Vector2 delta) { mPosition = Vector2Add(mPosition, delta); }
	void TranslateX(float delta) { mPosition.x += delta; }
	void TranslateY(float delta) { mPosition.y += delta; }
	void SetRotation(float rotation) { mRotation = rotation; }
	void Rotate(float delta) { mRotation += delta; }
	void SetScale(Vector2 scale) { mScale = scale; }
	void Scale(Vector2 delta) { mScale = Vector2Add(mScale, delta); }
	void ScaleX(float delta) { mScale.x += delta; }
	void ScaleY(float delta) { mScale.y += delta; }
	void SetColor(Color color) { mColor = color; }
	void SetAlpha(unsigned char alpha) { mColor.a = alpha; }

	void FlipX(bool flip) { mFlipX = flip; }
	void FlipY(bool flip) { mFlipY = flip; }
	void FlipX() { mFlipX = !mFlipX; }
	void FlipY() { mFlipY = !mFlipY; }


	Vector2 GetScale() const { return mScale; }
	Vector2 GetPosition() const { return mPosition; }
	float GetRotation() const { return mRotation; }
	Color GetColor() const { return mColor; }
	Texture2D GetTexture() const { return mTexture; }
	Type GetType() const { return mType; }

	const size_t GetID() const { return mId; }


	virtual void Draw();
	Rectangle Collider();
	Sprite(Type type, Texture2D texture, Vector2 position, Vector2 scale = {1, 1});
	Sprite(Type type, Vector2 position, Vector2 size, Color color = WHITE);
	Sprite() = delete;

	virtual ~Sprite() {}
protected:
	Texture2D mTexture;
	Vector2 mPosition;
	Vector2 mScale;
	float mRotation;
	Color mColor;
	Type mType;
	bool mFlipX;
	bool mFlipY;
private:
	bool mUsingTexture;
	const size_t mId;

	inline static size_t sId = 0;
};


#define SpriteConstructor(className) className(Sprite::Type type, Texture2D texture, Vector2 position, Vector2 scale = {1, 1}) : GM::Sprite(type, texture, position, scale)
#define SpriteConstructorCPP(className) className(Sprite::Type type, Texture2D texture, Vector2 position, Vector2 scale) : GM::Sprite(type, texture, position, scale)
#define SpriteHeaderConstructor(className) className(Sprite::Type type, Texture2D texture, Vector2 position, Vector2 scale = {1, 1})
#define SpriteOverladedConstructor(className, ...) className(Sprite::Type type, Texture2D texture, Vector2 position, Vector2 scale, __VA_ARGS__) : GM::Sprite(type, texture, position, scale)
#define SpriteHeaderOverloadedConstructor(className, ...) className(Sprite::Type type, Texture2D texture, Vector2 position, Vector2 scale , __VA_ARGS__)





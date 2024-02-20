#include "pch.h"
#include "Sprite.h"

void Sprite::Draw()
{
	if (mUsingTexture)
	{
		Rectangle source = { 0.0f, 0.0f, (float)mTexture.width, (float)mTexture.height };
		Rectangle dest = { mPosition.x, mPosition.y, mTexture.width * mScale.x, mTexture.height * mScale.y };
		Vector2 origin = { 0.0f, 0.0f };

		if (mFlipX)
			source.width = -source.width;
		if (mFlipY)
			source.height = -source.height;

		DrawTexturePro(mTexture, source, dest, origin, mRotation, mColor);
		DrawRectangleLinesEx(dest, 2.f, mColor);
	}
	else
	{
		Rectangle rec(mPosition.x, mPosition.y, mScale.x, mScale.y);
		DrawRectanglePro(rec, {}, mRotation, mColor);
	}
}

Rectangle Sprite::Collider()
{
	if (mUsingTexture)
	{
		Vector2 size = Vector2Multiply(Vector2(mTexture.width, mTexture.height), mScale);
		return Rectangle(mPosition.x, mPosition.y, size.x, size.y);
	}
	else
	{
		return Rectangle(mPosition.x, mPosition.y, mScale.x, mScale.y);
	}

}

Sprite::Sprite(Type type, Texture2D texture, Vector2 position, Vector2 scale)
	: mType(type), mTexture(texture), mPosition(position), 
	mScale(scale), mRotation(0), mUsingTexture(true), 
	mColor(WHITE), mFlipX(false), mFlipY(false), mId(sId++)
{

}
Sprite::Sprite(Type type, Vector2 position, Vector2 size, Color color)
	:  mType(type), mTexture({}), mPosition(position), 
	mScale(size), mRotation(0), mUsingTexture(false),
	mColor(color), mFlipX(false), mFlipY(false), mId(sId++)
{

}
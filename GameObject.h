#pragma once
#include "Vector2.h"
#include "Camera.h"
#include <SDL3/SDL.h>

// They will be circles
class GameObject {
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;

	SDL_Color m_color;
	SDL_FRect m_rect;
	SDL_Texture* m_texture;

public:
	GameObject(SDL_Texture* texture);

	const Vector2& GetPostion() const;
	void SetPosition(const Vector2& newPosition);

	float GetRadius() const;
	void SetRadius(float radius);

	const Vector2& GetVelocity() const;
	void SetVelocity(const Vector2& newVelocity);

	const SDL_Color& GetColor() const;
	void SetColor(const SDL_Color& newColor);

	const SDL_FRect& GetRect() const;
	void SetRect(const SDL_FRect& newRect);

	void Update(float deltaTime);
	void Draw(SDL_Renderer* renderer, const Camera& cam, const Vector2& windowDimensions) const;
};
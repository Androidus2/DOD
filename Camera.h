#pragma once
#include "Vector2.h"
#include <SDL3/SDL.h>

class Camera {
	Vector2 m_position;
	float m_zoom;

public:
	Camera();

	void HandleEvent(float deltaTime, SDL_Event* event);

	SDL_FRect worldToScreenCoordinates(const SDL_FRect& worldRect) const;
};
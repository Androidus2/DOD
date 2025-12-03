#pragma once
#include "Vector2.h"
#include <SDL3/SDL.h>

class Camera {
	Vector2 m_position;
	float m_zoom;

	Vector2 m_movementInput;
	int m_zoomInput;

	// Camera pan keys
	bool m_isUpPressed;
	bool m_isDownPressed;
	bool m_isLeftPressed;
	bool m_isRightPressed;

	// Zoom in / out keys
	bool m_isEqualPressed;
	bool m_isMinusPressed;

public:
	Camera();

	void HandleEvent(SDL_Event* event);

	void Update(float deltaTime);

	SDL_FRect worldToScreenCoordinates(const SDL_FRect& worldRect) const;
};
#include "Camera.h"
#include "Constants.h"
#include <iostream>


Camera::Camera() {
	m_zoom = 1.0f;
	m_position = { 0.0f, 0.0f };

	m_zoomInput = 0.0f;
	m_movementInput = { 0.0f, 0.0f };

	m_isEqualPressed = false;
	m_isMinusPressed = false;

	m_isUpPressed = false;
	m_isDownPressed = false;
	m_isLeftPressed = false;
	m_isRightPressed = false;
}

void Camera::HandleEvent(SDL_Event* event) {
	switch (event->type) {
	case SDL_EVENT_KEY_DOWN:
		if (event->key.key == SDLK_LEFT)
			m_isLeftPressed = true;
		if (event->key.key == SDLK_RIGHT)
			m_isRightPressed = true;
		if (event->key.key == SDLK_UP)
			m_isUpPressed = true;
		if (event->key.key == SDLK_DOWN)
			m_isDownPressed = true;
		if (event->key.key == SDLK_EQUALS)
			m_isEqualPressed = true;
		if (event->key.key == SDLK_MINUS)
			m_isMinusPressed = true;
		break;
	case SDL_EVENT_KEY_UP:
		if (event->key.key == SDLK_LEFT)
			m_isLeftPressed = false;
		if (event->key.key == SDLK_RIGHT)
			m_isRightPressed = false;
		if (event->key.key == SDLK_UP)
			m_isUpPressed = false;
		if (event->key.key == SDLK_DOWN)
			m_isDownPressed = false;
		if (event->key.key == SDLK_EQUALS)
			m_isEqualPressed = false;
		if (event->key.key == SDLK_MINUS)
			m_isMinusPressed = false;
		break;
	}

}

void Camera::Update(float deltaTime) {
	m_zoomInput = 0.0f;
	m_movementInput.x = 0.0f;
	m_movementInput.y = 0.0f;

	if (m_isUpPressed)
		m_movementInput.y -= 1;
	if (m_isDownPressed)
		m_movementInput.y += 1;
	if (m_isRightPressed)
		m_movementInput.x += 1;
	if (m_isLeftPressed)
		m_movementInput.x -= 1;
	
	if (m_isEqualPressed)
		m_zoomInput -= 1;
	if (m_isMinusPressed)
		m_zoomInput += 1;

	m_zoom *= (1.f - m_zoomInput * zoomSpeed * deltaTime);

	m_position += m_movementInput * panSpeed * deltaTime * (1.0f / m_zoom);
}

SDL_FRect Camera::worldToScreenCoordinates(const SDL_FRect& worldRect) const {
	SDL_FRect newRect;
	newRect.x = (worldRect.x - m_position.x) * m_zoom;
	newRect.y = (worldRect.y - m_position.y) * m_zoom;
	newRect.w = worldRect.w * m_zoom;
	newRect.h = worldRect.h * m_zoom;
	return newRect;
}
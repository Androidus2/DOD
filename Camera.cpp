#include "Camera.h"
#include "Constants.h"


Camera::Camera() {
	m_zoom = 1.0f;
	m_position = { 0.0f, 0.0f };
}

void Camera::HandleEvent(float deltaTime, SDL_Event* event) {
	switch (event->type) {
	case SDL_EVENT_KEY_DOWN:
		if (event->key.key == SDLK_LEFT)
			m_position.x -= panSpeed * deltaTime * (1.0f / m_zoom);
		if (event->key.key == SDLK_RIGHT)
			m_position.x += panSpeed * deltaTime * (1.0f / m_zoom);
		if (event->key.key == SDLK_UP)
			m_position.y -= panSpeed * deltaTime * (1.0f / m_zoom);
		if (event->key.key == SDLK_DOWN)
			m_position.y += panSpeed * deltaTime * (1.0f / m_zoom);
		if (event->key.key == SDLK_EQUALS)
			m_zoom *= 1.1f;
		if (event->key.key == SDLK_MINUS)
			m_zoom /= 1.1f;
		break;
	}
}

SDL_FRect Camera::worldToScreenCoordinates(const SDL_FRect& worldRect) const {
	SDL_FRect newRect;
	newRect.x = (worldRect.x - m_position.x) * m_zoom;
	newRect.y = (worldRect.y - m_position.y) * m_zoom;
	newRect.w = worldRect.w * m_zoom;
	newRect.h = worldRect.h * m_zoom;
	return newRect;
}
#include "GameObject.h"
#include "Utils.h"
#include "Game.h"
#include "Constants.h"

GameObject::GameObject(SDL_Texture* texture, float radius) {
    m_texture = texture;
    m_radius = radius;

    m_position.x = Utils::GenerateRandomFloat(0.0f, mapSize);
    m_position.y = Utils::GenerateRandomFloat(0.0f, mapSize);

    m_velocity.x = Utils::GenerateRandomFloat(-100.0f, 100.0f);
    m_velocity.y = Utils::GenerateRandomFloat(-100.0f, 100.0f);

    m_rect = { 0.0f, 0.0f, radius * 2, radius * 2};
    m_color = Utils::GenerateRandomColor();
}

const Vector2& GameObject::GetPostion() const {
    return m_position;
}
void GameObject::SetPosition(const Vector2& newPosition) {
    m_position = newPosition;

    m_rect.x = m_position.x - m_rect.w / 2;
    m_rect.y = m_position.y - m_rect.h / 2;
}

float GameObject::GetRadius() const {
    return m_radius;
}

const Vector2& GameObject::GetVelocity() const {
    return m_velocity;
}
void GameObject::SetVelocity(const Vector2& newVelocity) {
    m_velocity = newVelocity;
}

const SDL_Color& GameObject::GetColor() const {
    return m_color;
}
void GameObject::SetColor(const SDL_Color& newColor) {
    m_color = newColor;
}

const SDL_FRect& GameObject::GetRect() const {
    return m_rect;
}
void GameObject::SetRect(const SDL_FRect& newRect) {
    m_rect = newRect;
}

void GameObject::Update(float deltaTime) {
    if ((m_position.x < 0.0f && m_velocity.x < 0.0f) || (m_position.x > mapSize && m_velocity.x > 0.0f))
        m_velocity.x *= -1;
    if ((m_position.y < 0.0f && m_velocity.y < 0.0f) || (m_position.y > mapSize && m_velocity.y > 0.0f))
        m_velocity.y *= -1;

    SetPosition(m_position + m_velocity * deltaTime);
}

void GameObject::Draw(SDL_Renderer* renderer, const Camera& cam, const Vector2& windowDimensions) const {
    if (!m_texture) return;

    SDL_FRect screenRect = cam.worldToScreenCoordinates(m_rect);

    if (screenRect.x + screenRect.w < 0 || screenRect.x > windowDimensions.x || screenRect.y + screenRect.h < 0 || screenRect.y > windowDimensions.y)
        return;

    SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);

    SDL_RenderTexture(renderer, m_texture, nullptr, &screenRect);
}
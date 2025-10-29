#include "GameObject.h"
#include "Utils.h"

GameObject::GameObject(SDL_Texture* texture, float radius) {
    m_texture = texture;
    m_radius = radius;

    m_position.x = Utils::GenerateRandomFloat(0.0f, 5000.0f);
    m_position.y = Utils::GenerateRandomFloat(0.0f, 5000.0f);

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

void GameObject::Update(float deltaTime) {
    if ((m_position.x < 0.0f && m_velocity.x < 0.0f) || (m_position.x > 5000.0f && m_velocity.x > 0.0f))
        m_velocity.x *= -1;
    if ((m_position.y < 0.0f && m_velocity.y < 0.0f) || (m_position.y > 5000.0f && m_velocity.y > 0.0f))
        m_velocity.y *= -1;

    SetPosition(m_position + m_velocity * deltaTime);
}

void GameObject::Draw(SDL_Renderer* renderer, const Camera& cam) const {
    if (!m_texture) return;

    SDL_FRect screenRect = cam.worldToScreenCoordinates(m_rect);

    SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);

    SDL_RenderTexture(renderer, m_texture, nullptr, &screenRect);
}
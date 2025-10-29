#include "Utils.h"

const int collisionIterations = 1;

SDL_Color Utils::GenerateRandomColor() {
	Uint8 r = static_cast<Uint8>(rand() % 256);
	Uint8 g = static_cast<Uint8>(rand() % 256);
	Uint8 b = static_cast<Uint8>(rand() % 256);
	return { r, g, b, 255 };
}

float Utils::GenerateRandomFloat(float low, float high) {
	float generated = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	return generated;
}

bool Utils::IsColliding(const GameObject& a, const GameObject& b) {
	Vector2 diff = a.GetPostion() - b.GetPostion();
	float distanceSquared = diff.x * diff.x + diff.y * diff.y;
	float radiusSum = a.GetRadius() + b.GetRadius();
	return distanceSquared < radiusSum * radiusSum;
}
void Utils::ResolveCollision(GameObject& a, GameObject& b) {
	Vector2 diff = b.GetPostion() - a.GetPostion();
	float dist = diff.GetMagnitude();
	if (dist == 0.0f)
		return;
	float overlap = a.GetRadius() + b.GetRadius() - dist;
	if (overlap > 0.0f) {
		Vector2 correction = diff.GetNormalized() * (overlap / 2.0f);
		a.SetPosition(a.GetPostion() - correction);
		b.SetPosition(b.GetPostion() + correction);
	}
}
void Utils::SetVelocitiesAfterCollisionResolution(GameObject& a, GameObject& b) {
	Vector2 dir = (b.GetPostion() - a.GetPostion()).GetNormalized();

	float speedA = a.GetVelocity().GetMagnitude();
	float speedB = b.GetVelocity().GetMagnitude();

	a.SetVelocity(dir * -speedA);
	b.SetVelocity(dir * speedB);
}


void Utils::ResolveCollisions(std::vector<GameObject>& gameObjects) {
	for (int k = 0; k < collisionIterations; ++k) {
		for (int i = 0; i < gameObjects.size(); ++i) {
			for (int j = i + 1; j < gameObjects.size(); ++j) {
				if (IsColliding(gameObjects[i], gameObjects[j])) {
					ResolveCollision(gameObjects[i], gameObjects[j]);
					SetVelocitiesAfterCollisionResolution(gameObjects[i], gameObjects[j]);
				}
			}
		}
	}
}
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

bool Utils::_IsColliding(float aX, float aY, float aRadius, float bX, float bY, float bRadius) {
	float xDiff = aX - bX;
	float yDiff = aY - bY;
	float distanceSquared = xDiff * xDiff + yDiff * yDiff;
	float radiusSum = aRadius + bRadius;
	return distanceSquared < radiusSum * radiusSum;
}
bool Utils::_IsColliding(const GameObject& a, const GameObject& b) {
	return _IsColliding(a.GetPostion().x, a.GetPostion().y, a.GetRadius(), b.GetPostion().x, b.GetPostion().y, b.GetRadius());
}

void Utils::_ResolveCollision(float& aX, float& aY, float aRadius, float& bX, float& bY, float bRadius) {
	float xDiff = aX - bX;
	float yDiff = aY - bY;
	float dist = Vector2{ xDiff, yDiff }.GetMagnitude();
	if (dist == 0.0f)
		return;
	float overlap = aRadius + bRadius - dist;
	float xCorrection = xDiff / dist * overlap / 2.0f;
	float yCorrecttion = yDiff / dist * overlap / 2.0f;
	aX -= xCorrection;
	aY -= yCorrecttion;
	bX += xCorrection;
	bY -= yCorrecttion;
}
void Utils::_ResolveCollision(GameObject& a, GameObject& b) {
	float aX = a.GetPostion().x;
	float aY = a.GetPostion().y;

	float bX = b.GetPostion().x;
	float bY = b.GetPostion().y;

	_ResolveCollision(aX, aY, a.GetRadius(), bX, bY, b.GetRadius());

	a.SetPosition({ aX, aY });
	b.SetPosition({ bX, bY });
}

void Utils::_SetVelocitiesAfterCollisionResolution(float aX, float aY, float& aVelX, float& aVelY, float bX, float bY, float& bVelX, float& bVelY) {
	float dirX = bX - aX;
	float dirY = bY - aY;
	float dist = Vector2{ dirX, dirY }.GetMagnitude();
	if (dist == 0.0f)
		return;
	dirX /= dist;
	dirY /= dist;

	float speedA = -(Vector2{ aVelX, aVelY }.GetMagnitude());
	float speedB = Vector2{ bVelX, bVelY }.GetMagnitude();

	aVelX = dirX * speedA;
	aVelY = dirY * speedA;

	bVelX = dirX * speedB;
	bVelY = dirY * speedB;
}
void Utils::_SetVelocitiesAfterCollisionResolution(GameObject& a, GameObject& b) {
	float aVelX = a.GetVelocity().x;
	float aVelY = a.GetVelocity().y;
	float bVelX = b.GetVelocity().x;
	float bVelY = b.GetVelocity().y;
	_SetVelocitiesAfterCollisionResolution(a.GetPostion().x, a.GetPostion().y, aVelX, aVelY, b.GetPostion().x, b.GetPostion().y, bVelX, bVelY);
	a.SetVelocity({ aVelX, aVelY });
	b.SetVelocity({ aVelX, bVelY });
}

void Utils::ResolveCollisions(int numberOfObjects, float* positionsX, float* positionsY, float* velocitiesX, float* velocitiesY, float* radiuses) {
	for (int k = 0; k < collisionIterations; ++k) {
		for (int i = 0; i < numberOfObjects; ++i) {
			for (int j = i + 1; j < numberOfObjects; ++j) {
				if (_IsColliding(positionsX[i], positionsY[i], radiuses[i], positionsX[j], positionsY[j], radiuses[j])) {
					_ResolveCollision(positionsX[i], positionsY[i], radiuses[i], positionsX[j], positionsY[j], radiuses[j]);
					_SetVelocitiesAfterCollisionResolution(positionsX[i], positionsY[i], velocitiesX[i], velocitiesY[i],
						positionsX[j], positionsY[j], velocitiesX[j], velocitiesY[j]);
				}
			}
		}
	}
}
void Utils::ResolveCollisions(std::vector<GameObject>& gameObjects) {
	for (int k = 0; k < collisionIterations; ++k) {
		for (int i = 0; i < gameObjects.size(); ++i) {
			for (int j = i + 1; j < gameObjects.size(); ++j) {
				if (_IsColliding(gameObjects[i], gameObjects[j])) {
					_ResolveCollision(gameObjects[i], gameObjects[j]);
					_SetVelocitiesAfterCollisionResolution(gameObjects[i], gameObjects[j]);
				}
			}
		}
	}
}
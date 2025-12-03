#include "Utils.h"
#include "Constants.h"
#include <windows.h>
#include <psapi.h>

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

void Utils::_ResolveCollision(float& ax, float& ay, float ar,
	float& bx, float& by, float br)
{
	float dx = bx - ax;
	float dy = by - ay;
	float dist = sqrt(dx * dx + dy * dy);

	if (dist < 0.000001f) {
		// Avoid divide by zero: give a tiny separation
		dx = 0.00001f;
		dy = 0.0f;
		dist = 0.00001f;
	}

	float overlap = (ar + br) - dist;
	if (overlap <= 0) 
		return;

	float nx = dx / dist;
	float ny = dy / dist;

	// weight the correction by inverse radius (or mass)
	float wA = 1.0f / ar;
	float wB = 1.0f / br;
	float wSum = wA + wB;

	float moveA = overlap * (wA / wSum);
	float moveB = overlap * (wB / wSum);

	ax -= nx * moveA;
	ay -= ny * moveA;
	bx += nx * moveB;
	by += ny * moveB;
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

void Utils::_SetVelocitiesAfterCollisionResolution(
	float aX, float aY, float& aVelX, float& aVelY, float aRadius,
	float bX, float bY, float& bVelX, float& bVelY, float bRadius)
{
	// Treat radius as mass
	float mA = aRadius;
	float mB = bRadius;

	// Collision normal
	float nx = bX - aX;
	float ny = bY - aY;
	float dist = sqrt(nx * nx + ny * ny);
	if (dist < 0.000001f) 
		return;

	nx /= dist;
	ny /= dist;

	// Velocity components along the collision normal
	float va = aVelX * nx + aVelY * ny;
	float vb = bVelX * nx + bVelY * ny;

	// If they are separating, no need to resolve
	if (va - vb <= 0.0f) 
		return;

	// 1D elastic collision formula
	float newVa = (va * (mA - mB) + 2 * mB * vb) / (mA + mB);
	float newVb = (vb * (mB - mA) + 2 * mA * va) / (mA + mB);

	// Convert scalar normal impulses back into 2D velocities
	float impulseA = newVa - va;
	float impulseB = newVb - vb;

	aVelX += impulseA * nx;
	aVelY += impulseA * ny;

	bVelX += impulseB * nx;
	bVelY += impulseB * ny;
}
void Utils::_SetVelocitiesAfterCollisionResolution(GameObject& a, GameObject& b) {
	float aVelX = a.GetVelocity().x;
	float aVelY = a.GetVelocity().y;
	float bVelX = b.GetVelocity().x;
	float bVelY = b.GetVelocity().y;
	_SetVelocitiesAfterCollisionResolution(a.GetPostion().x, a.GetPostion().y, aVelX, aVelY, a.GetRadius(), b.GetPostion().x, b.GetPostion().y, bVelX, bVelY, b.GetRadius());
	a.SetVelocity({ aVelX, aVelY });
	b.SetVelocity({ bVelX, bVelY });
}

void Utils::ResolveCollisions(int numberOfObjects, float* positionsX, float* positionsY, float* velocitiesX, float* velocitiesY, float* radiuses) {
	for (int k = 0; k < collisionIterations; ++k) {
		for (int i = 0; i < numberOfObjects; ++i) {
			for (int j = i + 1; j < numberOfObjects; ++j) {
				if (_IsColliding(positionsX[i], positionsY[i], radiuses[i], positionsX[j], positionsY[j], radiuses[j])) {
					_ResolveCollision(positionsX[i], positionsY[i], radiuses[i], positionsX[j], positionsY[j], radiuses[j]);
					_SetVelocitiesAfterCollisionResolution(positionsX[i], positionsY[i], velocitiesX[i], velocitiesY[i], radiuses[i],
						positionsX[j], positionsY[j], velocitiesX[j], velocitiesY[j], radiuses[j]);
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

int Utils::GetCurrentMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
	{
		return pmc.WorkingSetSize; // bytes currently in RAM
	}
	return 0;
}
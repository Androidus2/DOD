#pragma once
#include <cstdlib>
#include <vector>
#include <SDL3/SDL.h>
#include "GameObject.h"

class Utils {
private:
	static bool _IsColliding(float aX, float aY, float aRadius, float bX, float bY, float bRadius);
	static bool _IsColliding(const GameObject& a, const GameObject& b);
	static void _ResolveCollision(float& aX, float& aY, float aRadius, float& bX, float& bY, float bRadius);
	static void _ResolveCollision(GameObject& a, GameObject& b);
	static void _SetVelocitiesAfterCollisionResolution(float aX, float aY, float& aVelX, float& aVelY, float bX, float bY, float& bVelX, float& bVelY);
	static void _SetVelocitiesAfterCollisionResolution(GameObject& a, GameObject& b);
public:
	static SDL_Color GenerateRandomColor();
	static float GenerateRandomFloat(float low, float high);

	static void ResolveCollisions(int numberOfObjects, float* positionsX, float* positionsY, float* velocitiesX, float* velocitiesY, float* radiuses);
	static void ResolveCollisions(std::vector<GameObject>& gameObjects);

	static int GetCurrentMemoryUsage();
};
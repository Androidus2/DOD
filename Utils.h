#pragma once
#include <cstdlib>
#include <vector>
#include <SDL3/SDL.h>
#include "GameObject.h"

class Utils {
private:
	static bool IsColliding(const GameObject& a, const GameObject& b);
	static void ResolveCollision(GameObject& a, GameObject& b);
	static void SetVelocitiesAfterCollisionResolution(GameObject& a, GameObject& b);
public:
	static SDL_Color GenerateRandomColor();
	static float GenerateRandomFloat(float low, float high);

	static void ResolveCollisions(std::vector<GameObject>& gameObjects);
};
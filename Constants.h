#pragma once

constexpr int collisionIterations = 1;
constexpr int maxObjectCount = 1000000;
constexpr float mapSize = 5000.f;
constexpr float panSpeed = 300.0f;
constexpr float zoomSpeed = 0.5f;

constexpr float gridCellSize = 20.0f;
constexpr int gridLength = int(mapSize / gridCellSize) + 1;

constexpr float minimumBallRadius = 0.5f;
constexpr float maximumBallRadius = 1.0f;
constexpr float maximumBallVelocity = 10.f;
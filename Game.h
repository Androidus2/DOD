#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "GameObject.h"

class Game {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;

    bool m_running;

    // Variables for calculating delta time
    Uint64 m_now;
    Uint64 m_last;
    double m_deltaTime;

    SDL_Texture* m_circleTexture;
    Camera m_cam;
    std::vector<GameObject> m_gameObjects;

    void CalculateDeltaTime();

    void HandleInput();
    void Update();
    void Draw() const;

public:
    bool Init();
    void Run();

    ~Game();
};
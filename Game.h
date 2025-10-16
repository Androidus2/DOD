#pragma once
#include <SDL3/SDL.h>

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

    // TODO: Move these to a generic scene container with some other classes
    // "Game" specific variables
    float m_squareSpeed;
    SDL_FRect m_square;
    Uint8 m_squareBlue;

    void CalculateDeltaTime();

    void HandleInput();
    void Update();
    void Draw() const;

public:
    bool Init();
    void Run();

    ~Game();
};
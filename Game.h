#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "GameObject.h"
#include "Camera.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

class Game {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;

    bool m_running;

    // Delta Time
    Uint64 m_now;
    Uint64 m_last;
    double m_deltaTime;

    SDL_Texture* m_circleTexture;
    Camera m_cam;

    // OOP simulation
    std::vector<GameObject> m_gameObjects;

    // DOD simulation
    float* m_dodPositionsX;
    float* m_dodPositionsY;
    float* m_dodVelocitiesX;
    float* m_dodVelocitiesY;
    float* m_dodRadiuses;
    SDL_Color* m_dodColors;
    SDL_FRect* m_dodRects;

    bool m_changedNumberOfObjects;
    int m_numberOfObjects;

    bool m_isCollisionEnabled;

    bool m_useOOP;

    void _ResetDOD();
    void _UpdateDOD();
    void _DrawDOD();

    void _SwitchToDOD();
    void _SwitchToOOP();

    void _CalculateDeltaTime();
    void _HandleInput();
    void _Update();
    void _Draw();

public:
    bool Init();
    void Run();
    ~Game();
};

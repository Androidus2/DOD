#include <iostream>
#include "Game.h"
#include "Utils.h"
#include <SDL3_image/SDL_image.h>

void Game::CalculateDeltaTime() {
    m_last = m_now;
    m_now = SDL_GetPerformanceCounter();
    m_deltaTime = (m_now - m_last) * 1000 / (double)SDL_GetPerformanceFrequency();
    m_deltaTime /= 1000.0;
}

void Game::HandleInput() {
    while (SDL_PollEvent(&m_event)) {
        if (m_event.type == SDL_EVENT_QUIT)
            m_running = false;
        m_cam.HandleEvent(m_deltaTime, &m_event);
    }
}
void Game::Update() {
    CalculateDeltaTime();
    std::cout <<  static_cast<int>(1.0f / m_deltaTime) << '\n';

    for (int i = 0; i < m_gameObjects.size(); ++i)
        m_gameObjects[i].Update(m_deltaTime);

    Utils::ResolveCollisions(m_gameObjects);
}
void Game::Draw() const {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    for (int i = 0; i < m_gameObjects.size(); ++i)
        m_gameObjects[i].Draw(m_renderer, m_cam);

    SDL_RenderPresent(m_renderer);
}

bool Game::Init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 0;
    }

    m_window = SDL_CreateWindow("DOD Engine", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 0;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 0;
    }

    m_circleTexture = IMG_LoadTexture(m_renderer, "circle.png");
    if (!m_circleTexture) {
        std::cerr << "Failed to load circle texture: " << SDL_GetError() << std::endl;
        return 0;
    }
    SDL_SetTextureBlendMode(m_circleTexture, SDL_BLENDMODE_BLEND);

    srand(time(NULL));

    m_now = SDL_GetPerformanceCounter();
    m_last = 0;
    m_deltaTime = 0;

    for (int i=0; i < 10000; i++)
        m_gameObjects.push_back({ m_circleTexture, 5.0f });
    return 1;
}
void Game::Run() {
    m_running = true;
    while (m_running) {
        HandleInput();
        Update();
        Draw();
    }
}

Game::~Game() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_circleTexture) {
        SDL_DestroyTexture(m_circleTexture);
    }
    SDL_Quit();
}

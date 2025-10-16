#include <iostream>
#include "Game.h"

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

        if (m_event.type == SDL_EVENT_KEY_DOWN) {
            if (m_event.key.key == SDLK_SPACE) {
                m_squareSpeed *= -1;
            }
        }
    }
}
void Game::Update() {
    CalculateDeltaTime();
    m_square.x += m_squareSpeed * m_deltaTime;

    int squareXInt = m_square.x;
    m_squareBlue = squareXInt % 256;
}
void Game::Draw() const {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 120, 0, m_squareBlue, 255);
    SDL_RenderFillRect(m_renderer, &m_square);

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

    m_now = SDL_GetPerformanceCounter();
    m_last = 0;
    m_deltaTime = 0;

    m_squareSpeed = 100.f;
    m_square = { -100.f, 250.f, 100.f, 100.f };
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
    SDL_Quit();
}

#include <iostream>
#include "Game.h"
#include "Utils.h"

void Game::_CalculateDeltaTime() {
    m_last = m_now;
    m_now = SDL_GetPerformanceCounter();
    m_deltaTime = (m_now - m_last) * 1000 / (double)SDL_GetPerformanceFrequency();
    m_deltaTime /= 1000.0;
}

void Game::_UpdateDOD() {
    for (int i = 0; i < m_numberOfObjects; ++i) {
        if ((m_dodPositionsX[i] < 0.0f && m_dodVelocitiesX[i] < 0.0f) || (m_dodPositionsX[i] > 5000.0f && m_dodVelocitiesX[i] > 0.0f))
            m_dodVelocitiesX[i] *= -1;
        if ((m_dodPositionsY[i] < 0.0f && m_dodVelocitiesY[i] < 0.0f) || (m_dodPositionsY[i] > 5000.0f && m_dodVelocitiesY[i] > 0.0f))
            m_dodVelocitiesY[i] *= -1;

        m_dodPositionsX[i] += m_dodVelocitiesX[i] * m_deltaTime;
        m_dodPositionsY[i] += m_dodVelocitiesY[i] * m_deltaTime;

        m_dodRects[i].x = m_dodPositionsX[i] - m_dodRects[i].w / 2;
        m_dodRects[i].y = m_dodPositionsY[i] - m_dodRects[i].h / 2;
    }
}

void Game::_DrawDOD() {
    for (int i = 0; i < m_numberOfObjects; ++i) {
        SDL_FRect screenRect = m_cam.worldToScreenCoordinates(m_dodRects[i]);

        SDL_SetTextureColorMod(m_circleTexture, m_dodColors[i].r, m_dodColors[i].g, m_dodColors[i].b);

        SDL_RenderTexture(m_renderer, m_circleTexture, nullptr, &screenRect);
    }
}

void Game::_SwitchToDOD() {
    for (int i = 0; i < m_numberOfObjects; ++i) {
        m_dodPositionsX[i] = m_gameObjects[i].GetPostion().x;
        m_dodPositionsY[i] = m_gameObjects[i].GetPostion().y;

        m_dodVelocitiesX[i] = m_gameObjects[i].GetVelocity().x;
        m_dodVelocitiesY[i] = m_gameObjects[i].GetVelocity().y;

        m_dodRadiuses[i] = m_gameObjects[i].GetRadius();

        m_dodColors[i] = m_gameObjects[i].GetColor();
        m_dodRects[i] = m_gameObjects[i].GetRect();
    }
}

void Game::_SwitchToOOP() {
    m_gameObjects.clear();
    for (int i = 0; i < m_numberOfObjects; ++i) {
        GameObject newObject{ m_circleTexture, m_dodRadiuses[i] };
        newObject.SetPosition(Vector2{ m_dodPositionsX[i], m_dodPositionsY[i] });
        newObject.SetVelocity(Vector2{ m_dodVelocitiesX[i], m_dodVelocitiesY[i] });
        newObject.SetColor(m_dodColors[i]);
        newObject.SetRect(m_dodRects[i]);
        m_gameObjects.push_back(newObject);
    }
}

void Game::_HandleInput() {
    // We are considering state updates as inputs, so they are going to be handled here
    if (m_changedNumberOfObjects) {
        m_changedNumberOfObjects = false;

        if(m_useOOP){
            if (m_numberOfObjects < m_gameObjects.size())
                m_gameObjects.erase(m_gameObjects.begin() + m_numberOfObjects, m_gameObjects.end());
            else {
                int difference = m_numberOfObjects - m_gameObjects.size();
                for (int i = 0; i < difference; ++i) {
                    m_gameObjects.push_back(GameObject{ m_circleTexture, 5.0f });
                }
            }
        }
    }

    while (SDL_PollEvent(&m_event)) {
        ImGui_ImplSDL3_ProcessEvent(&m_event);

        if (m_event.type == SDL_EVENT_QUIT)
            m_running = false;

        m_cam.HandleEvent(m_deltaTime, &m_event);
    }
}

void Game::_Update() {
    _CalculateDeltaTime();

    if (m_useOOP) {
        for (int i = 0; i < m_gameObjects.size(); ++i)
            m_gameObjects[i].Update(m_deltaTime);

        if (m_isCollisionEnabled)
            Utils::ResolveCollisions(m_gameObjects);
    }
    else {
        _UpdateDOD();

        if (m_isCollisionEnabled)
            Utils::ResolveCollisions(m_numberOfObjects, m_dodPositionsX, m_dodPositionsY, m_dodVelocitiesX, m_dodVelocitiesY, m_dodRadiuses);
    }
}

void Game::_Draw() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    if (m_useOOP) {
        for (int i = 0; i < m_gameObjects.size(); ++i)
            m_gameObjects[i].Draw(m_renderer, m_cam);
    }
    else {
        _DrawDOD();
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Performance");
    ImGui::Text("FPS: %.1f", 1.0 / m_deltaTime);
    if (ImGui::InputInt("Number of objects", &m_numberOfObjects))
        m_changedNumberOfObjects = true;
    ImGui::Checkbox("Enable collision", &m_isCollisionEnabled);
    if (ImGui::Checkbox("Use OOP", &m_useOOP)) {
        if (!m_useOOP)
            _SwitchToDOD();
        else
            _SwitchToOOP();
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);

    srand((unsigned)time(NULL));

    m_now = SDL_GetPerformanceCounter();
    m_last = 0;
    m_deltaTime = 0;

    m_numberOfObjects = 10000;
    m_isCollisionEnabled = true;
    m_useOOP = true;

    if (m_useOOP) {
        for (int i = 0; i < m_numberOfObjects; i++)
            m_gameObjects.push_back(GameObject{ m_circleTexture, 5.0f });
    }

    m_dodPositionsX = new float[100000];
    m_dodPositionsY = new float[100000];

    m_dodVelocitiesX = new float[100000];
    m_dodVelocitiesY = new float[100000];

    m_dodRadiuses = new float[100000];

    m_dodColors = new SDL_Color[100000];

    m_dodRects = new SDL_FRect[100000];

    for (int i = 0; i < 100000; ++i) {
        m_dodPositionsX[i] = Utils::GenerateRandomFloat(0.0f, 5000.0f);
        m_dodPositionsY[i] = Utils::GenerateRandomFloat(0.0f, 5000.0f);

        m_dodVelocitiesX[i] = Utils::GenerateRandomFloat(-100.0f, 100.0f);
        m_dodVelocitiesY[i] = Utils::GenerateRandomFloat(-100.0f, 100.0f);

        m_dodRadiuses[i] = 5.0f;

        m_dodRects[i] = { 0.0f, 0.0f, m_dodRadiuses[i] * 2, m_dodRadiuses[i] * 2 };
        m_dodColors[i] = Utils::GenerateRandomColor();
    }

    return 1;
}

void Game::Run() {
    m_running = true;
    while (m_running) {
        _HandleInput();
        _Update();
        _Draw();
    }
}

Game::~Game() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (m_circleTexture) 
        SDL_DestroyTexture(m_circleTexture);
    if (m_renderer) 
        SDL_DestroyRenderer(m_renderer);
    if (m_window) 
        SDL_DestroyWindow(m_window);

    SDL_Quit();
}

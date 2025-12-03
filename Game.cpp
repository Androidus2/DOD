#include <iostream>
#include "Game.h"
#include "Utils.h"
#include "Constants.h"


void Game::_CalculateDeltaTime() {
    m_last = m_now;
    m_now = SDL_GetPerformanceCounter();
    m_deltaTime = (m_now - m_last) * 1000 / (double)SDL_GetPerformanceFrequency();
    m_deltaTime /= 1000.0;
}

void Game::_ResetDOD() {
    if (m_dodPositionsX) {
        delete[] m_dodPositionsX;
        m_dodPositionsX = nullptr;
    }
    if (m_dodPositionsY) {
        delete[] m_dodPositionsY;
        m_dodPositionsY = nullptr;
    }
    if (m_dodRadiuses) {
        delete[] m_dodRadiuses;
        m_dodRadiuses = nullptr;
    }
    if (m_dodVelocitiesX) {
        delete[] m_dodVelocitiesX;
        m_dodVelocitiesX = nullptr;
    }
    if (m_dodVelocitiesY) {
        delete[] m_dodVelocitiesY;
        m_dodVelocitiesY = nullptr;
    }
    if (m_dodColors) {
        delete[] m_dodColors;
        m_dodColors = nullptr;
    }
    if (m_dodRects) {
        delete[] m_dodRects;
        m_dodRects = nullptr;
    }
}
void Game::_UpdateDOD() {
    for (int i = 0; i < m_numberOfObjects; ++i) {
        if ((m_dodPositionsX[i] < 0.0f && m_dodVelocitiesX[i] < 0.0f) || (m_dodPositionsX[i] > mapSize && m_dodVelocitiesX[i] > 0.0f))
            m_dodVelocitiesX[i] *= -1;
        if ((m_dodPositionsY[i] < 0.0f && m_dodVelocitiesY[i] < 0.0f) || (m_dodPositionsY[i] > mapSize && m_dodVelocitiesY[i] > 0.0f))
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

        if (screenRect.x + screenRect.w < 0 || screenRect.x > m_windowWidth || screenRect.y + screenRect.h < 0 || screenRect.y > m_windowHeight)
            continue;

        SDL_SetTextureColorMod(m_circleTexture, m_dodColors[i].r, m_dodColors[i].g, m_dodColors[i].b);

        SDL_RenderTexture(m_renderer, m_circleTexture, nullptr, &screenRect);
    }
}

void Game::_SwitchToDOD() {
    _ResetDOD();

    m_dodPositionsX = new float[maxObjectCount];
    m_dodPositionsY = new float[maxObjectCount];
    m_dodVelocitiesX = new float[maxObjectCount];
    m_dodVelocitiesY = new float[maxObjectCount];
    m_dodRadiuses = new float[maxObjectCount];
    m_dodColors = new SDL_Color[maxObjectCount];
    m_dodRects = new SDL_FRect[maxObjectCount];

    // For DOD we are always going to have the max object count
    for (int i = 0; i < maxObjectCount; ++i) {
        if (i < m_numberOfObjects) {
            // The first m_numberOfObjects objects can be copied from m_gameObjects
            m_dodPositionsX[i] = m_gameObjects[i].GetPostion().x;
            m_dodPositionsY[i] = m_gameObjects[i].GetPostion().y;

            m_dodVelocitiesX[i] = m_gameObjects[i].GetVelocity().x;
            m_dodVelocitiesY[i] = m_gameObjects[i].GetVelocity().y;

            m_dodRadiuses[i] = m_gameObjects[i].GetRadius();

            m_dodColors[i] = m_gameObjects[i].GetColor();
            m_dodRects[i] = m_gameObjects[i].GetRect();
        }
        else {
            // The other maxObjectCount - m_numberOfObjects objects need to be generated
            m_dodPositionsX[i] = Utils::GenerateRandomFloat(0.0f, mapSize);
            m_dodPositionsY[i] = Utils::GenerateRandomFloat(0.0f, mapSize);

            m_dodVelocitiesX[i] = Utils::GenerateRandomFloat(-maximumBallVelocity, maximumBallVelocity);
            m_dodVelocitiesY[i] = Utils::GenerateRandomFloat(-maximumBallVelocity, maximumBallVelocity);

            m_dodRadiuses[i] = Utils::GenerateRandomFloat(minimumBallRadius, maximumBallRadius);

            m_dodRects[i] = { 0.0f, 0.0f, m_dodRadiuses[i] * 2, m_dodRadiuses[i] * 2 };
            m_dodColors[i] = Utils::GenerateRandomColor();
        }
    }

    m_gameObjects.clear();
}
void Game::_SwitchToOOP() {
    m_gameObjects.clear();
    for (int i = 0; i < m_numberOfObjects; ++i) {
        GameObject newObject{ m_circleTexture };
        newObject.SetPosition(Vector2{ m_dodPositionsX[i], m_dodPositionsY[i] });
        newObject.SetVelocity(Vector2{ m_dodVelocitiesX[i], m_dodVelocitiesY[i] });
        newObject.SetColor(m_dodColors[i]);
        newObject.SetRect(m_dodRects[i]);
        m_gameObjects.push_back(newObject);
    }
    _ResetDOD();
}

void Game::_HandleInput() {
    // We are considering state updates as inputs, so they are going to be handled here
    if (m_changedNumberOfObjects) {
        m_changedNumberOfObjects = false;

        if (m_numberOfObjects > maxObjectCount)
            m_numberOfObjects = maxObjectCount;

        if(m_useOOP){
            if (m_numberOfObjects < m_gameObjects.size())
                m_gameObjects.erase(m_gameObjects.begin() + m_numberOfObjects, m_gameObjects.end());
            else {
                int difference = m_numberOfObjects - m_gameObjects.size();
                for (int i = 0; i < difference; ++i) {
                    m_gameObjects.push_back(GameObject{ m_circleTexture });
                }
            }
        }
    }

    while (SDL_PollEvent(&m_event)) {
        ImGui_ImplSDL3_ProcessEvent(&m_event);

        if (m_event.type == SDL_EVENT_QUIT)
            m_running = false;

        if(m_event.type == SDL_EVENT_WINDOW_RESIZED)
            SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

        m_cam.HandleEvent(&m_event);
    }
}
void Game::_Update() {
    _CalculateDeltaTime();
    m_cam.Update(m_deltaTime);

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
        Vector2 windowDimensions{ m_windowWidth, m_windowHeight };
        for (int i = 0; i < m_gameObjects.size(); ++i)
            m_gameObjects[i].Draw(m_renderer, m_cam, windowDimensions);
    }
    else {
        _DrawDOD();
    }

    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Performance");
    ImGui::Text("FPS: %.1f", 1.0 / m_deltaTime);
    ImGui::Text("Memory Usage: %.2f MB", Utils::GetCurrentMemoryUsage() / (1024.0 * 1024.0));
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
    m_windowHeight = 600;
    m_windowWidth = 800;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 0;
    }

    m_window = SDL_CreateWindow("DOD Engine", m_windowWidth, m_windowHeight, SDL_WINDOW_RESIZABLE);
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

    for (int i = 0; i < m_numberOfObjects; i++)
        m_gameObjects.push_back(GameObject{ m_circleTexture });

    if (!m_useOOP)
        _SwitchToDOD();

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
    _ResetDOD();

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

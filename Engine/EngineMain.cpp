#include "EngineMain.h"
#include "WindowManager.h"
#include "GameManager.h"

GameManager* Manager = nullptr;

void InitializeEngine(int argc, char** argv)
{


    WindowManager::Initialize(
        "Diabolical Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, SDL_WINDOW_HIDDEN);

    Manager = new GameManager();
}

void RunEngineLoop()
{
    Check(Manager);
    Manager->MainGameLoop();
}
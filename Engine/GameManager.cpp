#include "GameManager.h"
#include "WindowManager.h"
#include "Logging/Logging.h"
#include "Rendering/RenderingSystems.h"
#include <chrono>
#include "SDL2/SDL.h"
#include "ECS/flecs.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"
#include "Rendering/ECSImGuiTest/ImGuiECS.h"

float GameManager::FPS = 0;
float GameManager::GameTime = 0;
int GameManager::Frame = 0;
flecs::world ecs;

void InitializeImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)WindowManager::GetWindowSize().width, (float)WindowManager::GetWindowSize().height);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplSDL2_InitForOpenGL(WindowManager::GetSDLWindow(), WindowManager::GetGLContext());
}

void StartNewFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(WindowManager::GetSDLWindow());
}

void InitializeTestECS()
{
	ecs.entity("ImPooi")
		.add<ImGuiECSTest::ImGuiTestComponent>();	

	ecs.system<ImGuiECSTest::ImGuiTestComponent>()
		.each(ImGuiECSTest::ImGuiTestSystem);
}

void GameManager::MainGameLoop()
{
	LOGVERBOSE("GameManager::MainGameLoop()", "Main game loop started");
	glClearColor(0.f, 0.f, 0.5f, 1.0f);

	InitializeImGui();
		
	InitializeTestECS();

	// Play one frame before showing the window so that we can avoid the white screen of death
	StartNewFrame();
	EventTick();
	ManagerTick();
	EndFrame();
	// ===========================

	SDL_ShowWindow(WindowManager::GetSDLWindow());
	SDL_GL_SetSwapInterval(-1);
	while (bMainLoopRunning)
	{
		StartNewFrame();
		EventTick();
		ManagerTick();
		EndFrame();
	}

	ImGui::DestroyContext();
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				break;
		default:
			break;
		}
	}
}

void GameManager::ManagerTick()
{

	static long long OldTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long Time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long TimeDiff = Time - OldTime;
	OldTime = Time;

	float DeltaTime = (float)TimeDiff / 1000000000.f;
	FPS = 1.f / DeltaTime;

	ecs.progress(DeltaTime);

	GameTime += DeltaTime;

}

int GameManager::GetFrame()
{
	return Frame;
}

float GameManager::GetFPS()
{
	return FPS;
}

float GameManager::GetTime()
{
	return GameTime;
}
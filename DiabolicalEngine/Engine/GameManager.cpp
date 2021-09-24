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
#include <AL/al.h>
#include <AL/alc.h>
#include "Assets/TextAsset.h"
#include "Assets/GameAssetSoftPointer.h"

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
	GameAssetSoftPointer<TextAsset> TestTextSoftPointer = GameAssetSoftPointer<TextAsset>("EngineAssetFiles/TestText.txt");
	TextAsset* TestText = TestTextSoftPointer.LoadSynchronous();

	ecs.entity("ImPooi")
		.set([=](ImGuiECSTest::ImGuiTestComponent& p) { p.WindowText = TestText->GetString(); });




	ecs.system<ImGuiECSTest::ImGuiTestComponent>()
		.each(ImGuiECSTest::ImGuiTestSystem);
}

void InitializeOpenAL()
{
	ALCdevice* device;
	device = alcOpenDevice(NULL);
	if (!device)
	{
		LOGERROR("alc device null");
		return;
	}

	ALCcontext* context;

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		LOGERROR("alc failed to make context current");
		return;
	}

	ALfloat listenerOrientation[] = { 0.f, 0.f, 1.f, 0.f, 0.f, 1.f };

	alListener3f(AL_POSITION, 0, 0, 1.f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOrientation);

	ALuint source;

	alGenSources((ALuint)1, &source);

	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);
}


void GameManager::MainGameLoop()
{
	LOGVERBOSE("Main game loop started");
	glClearColor(0.f, 0.f, 0.5f, 1.0f);
	
	InitializeOpenAL();

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

void ShaderPractice() {
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"out vec3 FragColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"FragColor = vec3(position.x, position.y, position.z);\n"
		"}\0";

	unsigned int VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(VertexShader);

	int Success;
	char InfoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		LOGERROR("Vertex Shader: " + std::string(InfoLog));
	}

	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 FragColor;"
		"void main()\n"
		"{\n"
		"color = vec4(FragColor, 1.0f);\n"
		"}\n\0";

	unsigned int FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
		LOGERROR("Fragment Shader: " + std::string(InfoLog));
		glDeleteShader(VertexShader);
	}

	GLuint ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		LOGERROR("Shader Program: " + std::string(InfoLog));
		glDeleteShader(VertexShader); // delete shaders after shader program is created
		glDeleteShader(FragmentShader);
	}

	glDeleteShader(VertexShader); // delete shaders after shader program is created
	glDeleteShader(FragmentShader);

	// ======================================

	float Vertices[] = {
	 0.5f,  0.5f, 0.0f,// bottom right
	 0.5f, -0.5f, 0.0f,// top right
	-0.5f, -0.5f, 0.0f,// top left
	-0.5f,  0.5f, 0.0f, // bottom left 
	};

	unsigned int Indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint VAO, VBO;
	GLuint EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	{//OnRender
	glBindVertexArray(VAO);
	glUseProgram(ShaderProgram);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
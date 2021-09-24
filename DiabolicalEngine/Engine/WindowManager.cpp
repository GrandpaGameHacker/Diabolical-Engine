#include "WindowManager.h"

#include "WindowManager.h"
#include "Logging/Logging.h"
#include "EngineMain.h"
#include <gl/glew.h>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

WindowManager* WindowManager::Singleton;
SDL_Window* WindowManager::GameWindow;
SDL_GLContext WindowManager::GameGLContext;
WindowSize WindowManager::WinSize;

WindowManager::WindowManager(const char* title, int x, int y, int w, int h, Uint32 flags) :
	WindowFlags(flags), bWindowValid(true)
{

	FixWindowsHighDPIScaling();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		LOGERROR("SDL_Init() failed!");
		bWindowValid = false;
	}
	LOG("SDL_Init() success");

	GameWindow = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	GameGLContext = SDL_GL_CreateContext(GameWindow);
	if (!GameGLContext) {
		LOGERROR("SDL_Init() failed!");
		bWindowValid = false;
	}
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WindowManager::Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags)
{
	static bool bInitialized = false;

	Check(!bInitialized);
	bInitialized = true;
	Singleton = new WindowManager(WindowTitle, x, y, w, h, WindowFlags);
}

void WindowManager::FixWindowsHighDPIScaling()
{

#ifdef PLATFORM_WINDOWS
	typedef enum PROCESS_DPI_AWARENESS {
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;

	void* userDLL;
	BOOL(WINAPI * SetProcessDPIAware)(void) = nullptr; // Vista and later
	void* shcoreDLL;
	HRESULT(WINAPI * SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness) = nullptr; // Windows 8.1 and later

	userDLL = SDL_LoadObject("USER32.DLL");
	if (userDLL) {
		SetProcessDPIAware = (BOOL(WINAPI*)(void)) SDL_LoadFunction(userDLL, "SetProcessDPIAware");
	}

	shcoreDLL = SDL_LoadObject("SHCORE.DLL");
	if (shcoreDLL) {
		SetProcessDpiAwareness = (HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
	}

	if (SetProcessDpiAwareness) {
		/* Try Windows 8.1+ version */
		HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		SDL_Log("called SetProcessDpiAwareness: %d", (result == S_OK) ? 1 : 0);
	}
	else if (SetProcessDPIAware) {
		/* Try Vista - Windows 8 version.
		This has a constant scale factor for all monitors.
		*/
		BOOL success = SetProcessDPIAware();
		SDL_Log("called SetProcessDPIAware: %d", (int)success);
	}
#endif // PLATFORM_WINDOWS
}

WindowManager::~WindowManager()
{
	SDL_GL_DeleteContext(GameGLContext);
	SDL_DestroyWindow(GameWindow);
	SDL_Quit();
}

SDL_Window* WindowManager::GetSDLWindow()
{
	Check(GameWindow);
	return GameWindow;
}

SDL_GLContext WindowManager::GetGLContext()
{
	Check(GameGLContext);
	return GameGLContext;
}

bool WindowManager::IsValid()
{
	return bWindowValid;
}

WindowSize WindowManager::GetWindowSize()
{
	return WinSize;
}

WindowManager& WindowManager::Get()
{
	Check(Singleton);
	return *Singleton;
}

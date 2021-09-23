#include "Diabolical.h"
#include "Engine/EngineMain.h"
#include "Engine/Logging/Logging.h"

#include <iostream>

#undef main
int main(int argc, char* argv[]) {
	InitializeEngine(argc, argv);
	RunEngineLoop();
}
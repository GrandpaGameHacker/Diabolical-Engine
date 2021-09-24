#include "Diabolical.h"
#include "Engine/EngineMain.h"
#include "Engine/Logging/Logging.h"

#include <iostream>

#undef main
int main(int argc, char* argv[]) {
	Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);
	InitializeEngine(argc, argv);
	RunEngineLoop();
}
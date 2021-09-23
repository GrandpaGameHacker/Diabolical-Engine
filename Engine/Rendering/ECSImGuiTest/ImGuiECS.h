#pragma once
#include <string>

namespace ImGuiECSTest
{
	struct ImGuiTestComponent
	{
		std::string WindowName = "Window";
		std::string WindowText = "Content";
	};

	extern void ImGuiTestSystem(ImGuiTestComponent& component);

}
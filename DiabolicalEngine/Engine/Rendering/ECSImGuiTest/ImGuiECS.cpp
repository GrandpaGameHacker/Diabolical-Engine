#include "ImGuiECS.h"
#include "../imgui/imgui.h"

void ImGuiECSTest::ImGuiTestSystem(ImGuiTestComponent& component)
{
	ImGui::Begin(component.WindowName.c_str());
	ImGui::Text(component.WindowText.c_str());
	ImGui::End();
}
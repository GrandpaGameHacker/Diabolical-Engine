#include "ImGuiECS.h"
#include "../imgui/imgui.h"
#include "../../ECS/flecs.h"
#include "../../FunctionInvoker.h"
#include "../../Logging/Logging.h"
extern flecs::world ecs;


void ImGuiECSTest::ImGuiTestSystem(ImGuiTestComponent& component)
{
	ImGui::Begin((component.WindowName).c_str());
	ImGui::Text(component.WindowText.c_str());
	if (ImGui::Button("shit out another window"))
	{
		
		FunctionInvoker::Defer([]() {

			ecs.entity()
				.set([=](ImGuiECSTest::ImGuiTestComponent& p)
			{
				static int poop = 0;
				poop++;
				p.WindowText = "New Window content haha smd";
				p.WindowName = "new window##" + std::to_string(poop);
			});
		});

	}
	ImGui::End();
}
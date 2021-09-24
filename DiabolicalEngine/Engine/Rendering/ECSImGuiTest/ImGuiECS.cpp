#include "ImGuiECS.h"
#include "../imgui/imgui.h"
#include "../../ECS/flecs.h"
#include "../../FunctionInvoker.h"
#include "../../Logging/Logging.h"
#include <AL/al.h>
extern flecs::world ecs;

extern float Divisor;
extern ALuint memebuffer;
extern ALuint memesource;
extern char* memeaudio;
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

	if (ImGui::SliderFloat("Divisor", &Divisor, 0, 100))
	{
		for (float a = 0; a < 48000; a++)
		{
			memeaudio[(int)a] = (char)((cos(a / Divisor) + 1) * 60);
		}


		alSourceStop(memesource);
		alSourcei(memesource, AL_BUFFER, NULL);

		alDeleteBuffers(1, &memebuffer);

		alGenBuffers(1, &memebuffer);

		alBufferData(memebuffer, AL_FORMAT_MONO8, memeaudio, 48000, 48000);
		alSourcei(memesource, AL_BUFFER, memebuffer);

		alSourcePlay(memesource);
	}
	ImGui::End();
}
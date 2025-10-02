#include "client.h"
#include "globals.h"
#include "immapp/immapp.h"
#include "imgui.h"
#include "ixwebsocket/IXNetSystem.h"


static void handle_gui_ws_test_buttons()
{
	ImGui::BeginDisabled(init_ws_test_disable);
	if (ImGui::Button("Initialize"))
		init_ws_test();
	
	ImGui::EndDisabled();
	ImGui::SameLine();

	ImGui::BeginDisabled(activate_ws_test_disable);
	if (ImGui::Button("Activate"))
		start_ws_test();
	
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(deactivate_ws_test_disable);
	if (ImGui::Button("Deactivate"))
		stop_ws_test();
	
	ImGui::EndDisabled();
}

static void gui()
{
    ImGui::Text("This program does nothing at the moment but try to simulate a proper websocket connection to a vm on collabvm.");

	ImGui::Separator();
	ImGui::Text("WS Test: ");
	ImGui::SameLine();
    handle_gui_ws_test_buttons();

    ImGui::Separator();
    HelloImGui::LogGui();
}

int main(int, char**)
{
#ifdef _WIN32 	// Required on Windows for websocket shit.
    ix::initNetSystem();
#endif

    ImmApp::Run(gui,g_window_title,true,true);

#ifdef _WIN32
    ix::uninitNetSystem();
#endif
    return 0;
}
#include "client.h"
#include "guac.h"
#include "imgui.h"
#include "immapp/immapp.h"
#include "ixwebsocket/IXNetSystem.h"

static void ws_test_gui()
{
	ImGui::Separator();

	ImGui::BeginDisabled(ui::globals::activate_ws_disable);
	if (ImGui::Button("Connect"))
		client::start_ws();
	
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(ui::globals::deactivate_ws_disable);
	if (ImGui::Button("Deactivate"))
		client::stop_ws();
	
	ImGui::EndDisabled();

	ImGui::Text("CVM WS Server URL: ");
	ImGui::SameLine();
	ImGui::InputText("##url", client::globals::url, IM_ARRAYSIZE(client::globals::url));

	ImGui::Separator();
}

static void gui()
{
    ImGui::Text("This program does nothing at the moment but try to simulate a proper websocket connection to a vm on collabvm.");

    ws_test_gui();

    HelloImGui::LogGui();
}

int main(int, char**)
{
#ifdef _WIN32 	// Required on Windows for websocket shit.
    ix::initNetSystem();
#endif

	// Init WS
	client::init_ws_handler();

    ImmApp::Run(gui, ui::globals::window_title,true,true);

#ifdef _WIN32
    ix::uninitNetSystem();
#endif
    return 0;
}
#include "client.h"
#include "guac.h"
#include "imgui.h"
#include "immapp/immapp.h"
#include "ixwebsocket/IXNetSystem.h"

static void ws_test_gui()
{
	ImGui::Separator();

	ImGui::BeginDisabled(ui::g_activate_ws_disable);
	if (ImGui::Button("Connect"))
		client::start_ws();
	
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(ui::g_deactivate_ws_disable);
	if (ImGui::Button("Deactivate"))
		client::stop_ws();
	
	ImGui::EndDisabled();

	ImGui::Text("CVM WS Server URL: ");
	ImGui::SameLine();
	ImGui::InputText("##url", client::g_url, IM_ARRAYSIZE(client::g_url));

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

    ImmApp::Run(gui, ui::g_window_title,true,true);

#ifdef _WIN32
    ix::uninitNetSystem();
#endif
    return 0;
}
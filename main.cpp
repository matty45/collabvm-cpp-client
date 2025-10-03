#include "client.h"
#include "globals.h"
#include "guac.h"
#include "immapp/immapp.h"
#include "imgui.h"
#include "ixwebsocket/IXNetSystem.h"

static void ws_test_gui()
{
	ImGui::Separator();

	ImGui::Text("Websocket Testing");

	if (ImGui::Button("WS Parse Test"))
		guac_decode("7.adduser,1.9,4.gogo,1.0,3.mv_,1.0,7.moniker,1.0,10.yellows111,1.0,10.guest53144,1.0,11.LandOfKurac,1.0,4.manu,1.0,10.guest74773,1.0,14.JoaquimNicacio,1.0;");

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

	ImGui::Text("CVM WS Server URL: ");
	ImGui::SameLine();
	ImGui::InputText("##url", ws_url, IM_ARRAYSIZE(ws_url));

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

    ImmApp::Run(gui,g_window_title,true,true);

#ifdef _WIN32
    ix::uninitNetSystem();
#endif
    return 0;
}
#include "client.h"
#include "globals.h"
#include "immapp/immapp.h"
#include "imgui.h"
#include "ixwebsocket/IXNetSystem.h"


bool ws_test_disable = false;

static void gui()
{
    ImGui::Text("This program does nothing at the moment but try to simulate a proper websocket connection to a vm on collabvm.");

    ImGui::BeginDisabled(ws_test_disable);
    if (ImGui::Button("Activate WS Test."))
    {
        ws_test();
        ws_test_disable = true;
    }
    ImGui::EndDisabled();

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
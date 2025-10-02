#include "client.h"
#include "globals.h"
#include "immapp/immapp.h"
#include "imgui.h"


bool ws_test_disable = false;

static void gui()
{
    ImGui::Text("This program does nothing at the moment but try to simulate a proper websocket connection to a vm on collabvm.");

    ImGui::BeginDisabled(ws_test_enable);
    if (ImGui::Button("Activate WS Test."))
    {
        ws_test();
        ws_test_enable = true;
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    HelloImGui::LogGui();
}

int main(int, char**)
{
    ImmApp::Run(gui,g_window_title,true,true);

    return 0;
}
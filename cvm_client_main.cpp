#include "hello_imgui/hello_imgui.h"
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"


void test_func()
{

}

int main(int, char* [])
{
#ifdef ASSETS_LOCATION
	HelloImGui::SetAssetsFolder(ASSETS_LOCATION);
#endif
	auto guiFunction = []() {
		ImGui::Text("Hello, ");                    // Display a simple label
		HelloImGui::ImageFromAsset("world.jpg");   // Display a static image
		if (ImGui::Button("Test!"))
			test_func();
		};
	HelloImGui::Run(guiFunction, "Hello, globe", true);
	return 0;
}

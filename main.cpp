#include <vector>

#include "client.h"
#include "hello_imgui/hello_imgui.h"
#include "hello_imgui/runner_params.h"
#include "hello_imgui/internal/functional_utils.h"
#include "ixwebsocket/IXNetSystem.h"


// Define the default docking splits for the application layout
inline std::vector<HelloImGui::DockingSplit> create_default_docking_splits()
{
	// Create docking splits  
	// Split bottom for log window (25% height)  
	HelloImGui::DockingSplit splitLog;
	splitLog.initialDock = "MainDockSpace";
	splitLog.newDock = "LogSpace";
	splitLog.direction = ImGuiDir_Down;
	splitLog.ratio = 0.25f;

	// Split right for user list (20% width)  
	HelloImGui::DockingSplit splitUserList;
	splitUserList.initialDock = "MainDockSpace";
	splitUserList.newDock = "UserListSpace";
	splitUserList.direction = ImGuiDir_Right;
	splitUserList.ratio = 0.2f;

	return { splitLog, splitUserList };
}

std::vector<std::string> get_statically_ordered_layouts_list(const HelloImGui::RunnerParams& runnerParams)
{
	static std::vector<std::string> staticallyOrderedLayoutNames;

	// First fill the static vector with currently available layouts
	if (!HelloImGui::FunctionalUtils::vector_contains(staticallyOrderedLayoutNames, runnerParams.dockingParams.layoutName))
		staticallyOrderedLayoutNames.push_back(runnerParams.dockingParams.layoutName);
	for (const auto& layout : runnerParams.alternativeDockingLayouts)
		if (!HelloImGui::FunctionalUtils::vector_contains(staticallyOrderedLayoutNames, layout.layoutName))
			staticallyOrderedLayoutNames.push_back(layout.layoutName);

	// Then, fill currently available layouts
	std::vector<std::string> currentLayoutNames;
	currentLayoutNames.push_back(runnerParams.dockingParams.layoutName);
	for (const auto& layout : runnerParams.alternativeDockingLayouts)
		currentLayoutNames.push_back(layout.layoutName);

	// Only display currently available layout, but with the static order
	std::vector<std::string> layoutNames;
	for (const auto& staticalLayoutName : staticallyOrderedLayoutNames)
	{
		if (HelloImGui::FunctionalUtils::vector_contains(currentLayoutNames, staticalLayoutName))
			layoutNames.push_back(staticalLayoutName);
	}

	return layoutNames;
}

void MenuView_Layouts(HelloImGui::RunnerParams& runnerParams)
{
	bool hasAlternativeDockingLayouts = (runnerParams.alternativeDockingLayouts.size() > 0);

	if (hasAlternativeDockingLayouts)
		ImGui::SeparatorText("Layouts");

	if (!runnerParams.dockingParams.dockableWindows.empty())
		if (ImGui::MenuItem("Restore default layout##szzz"))
			runnerParams.dockingParams.layoutReset = true;

	ImGui::PushID("Layouts##asldqsl");

	if (hasAlternativeDockingLayouts)
	{
		if (ImGui::BeginMenu("Select Layout"))
		{
			auto layoutNames = get_statically_ordered_layouts_list(runnerParams);
			for (const auto& layoutName : layoutNames)
			{
				bool isSelected = (layoutName == runnerParams.dockingParams.layoutName);
				if (ImGui::MenuItem(layoutName.c_str(), nullptr, isSelected))
					HelloImGui::SwitchLayout(layoutName);
			}
			ImGui::EndMenu();
		}
	}

	ImGui::PopID();
}

void MenuView_DockableWindows(HelloImGui::RunnerParams& runnerParams)
{
	auto& dockableWindows = runnerParams.dockingParams.dockableWindows;
	if (dockableWindows.empty())
		return;

	ImGui::PushID("DockableWindows##asldqsl");

	ImGui::SeparatorText("Windows");


	if (ImGui::MenuItem("View All##DSQSDDF"))
		for (auto& dockableWindow : runnerParams.dockingParams.dockableWindows)
			if (dockableWindow.canBeClosed && dockableWindow.includeInViewMenu)
				dockableWindow.isVisible = true;
	if (ImGui::MenuItem("Hide All##DSQSDDF"))
		for (auto& dockableWindow : runnerParams.dockingParams.dockableWindows)
			if (dockableWindow.canBeClosed && dockableWindow.includeInViewMenu)
				dockableWindow.isVisible = false;

	{
		for (auto& dockableWindow : runnerParams.dockingParams.dockableWindows)
		{
			if (!dockableWindow.includeInViewMenu)
				continue;
			if (dockableWindow.canBeClosed)
			{
				if (ImGui::MenuItem(dockableWindow.label.c_str(), nullptr, dockableWindow.isVisible))
					dockableWindow.isVisible = !dockableWindow.isVisible;
			}
			else
			{
				ImGui::MenuItem(dockableWindow.label.c_str(), nullptr, dockableWindow.isVisible, false);
			}
		}
	}

	ImGui::PopID();
}

void theme_stuff()
{
	auto& tweakedTheme = HelloImGui::GetRunnerParams()->imGuiWindowParams.tweakedTheme;

	if (ImGui::BeginMenu("Theme"))
	{
		if (ImGui::MenuItem("Theme tweak window", nullptr, ui::g_show_theme_tweak_window))
			ui::g_show_theme_tweak_window = !ui::g_show_theme_tweak_window;

		ImGui::Separator();
		for (int i = 0; i < ImGuiTheme::ImGuiTheme_Count; ++i)
		{
			ImGuiTheme::ImGuiTheme_ theme = (ImGuiTheme::ImGuiTheme_)(i);
			bool selected = (theme == tweakedTheme.Theme);
			if (ImGui::MenuItem(ImGuiTheme::ImGuiTheme_Name(theme), nullptr, selected))
			{
				tweakedTheme.Theme = theme;
				ImGuiTheme::ApplyTheme(theme);
			}
		}
		ImGui::EndMenu();
	}

	ImGui::EndMenu();
}

void app_menu_gui()
{
	if (ImGui::MenuItem("Disconnect"))
	{
		client::stop_ws();
		ui::g_open_connection_window = true;
	}
}

inline void menu_gui(HelloImGui::RunnerParams& runnerParams)
{
	HelloImGui::ShowAppMenu(runnerParams);

	if (ImGui::BeginMenu("View"))
	{
		MenuView_Layouts(runnerParams);
		MenuView_DockableWindows(runnerParams);

		ImGui::SeparatorText("Other");
		theme_stuff();
	}

	//if (ImGui::BeginMenu("My Menu"))
	//{
	//	bool clicked = ImGui::MenuItem("Test me", "", false);
	//	if (clicked)
	//	{
	//		HelloImGui::Log(HelloImGui::LogLevel::Warning, "It works");
	//	}
	//	ImGui::EndMenu();
	//}
}

inline void chatbox_gui()
{
	ImGui::Text("Chatbox goes here.");
}

inline void user_list_gui()
{
	ImGui::Text("User list goes here.");
}


inline void settings_and_stuff_gui()
{
	HelloImGui::ShowThemeTweakGuiWindow(&ui::g_show_theme_tweak_window); // Handle theme tweaking window


	// Uglu popup code
	if (ui::g_open_connection_window == true) {
		ImGui::OpenPopup("About");
		ui::g_open_connection_window = false;
	}


	// Keep the modal centered at all times.
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{

		ImGui::Text("CVM WS Server URL: ");
		ImGui::SameLine();
		ImGui::InputText("##url", client::g_url, IM_ARRAYSIZE(client::g_url));

		ImGui::Text("Username: ");
		ImGui::SameLine();
		ImGui::InputText("##username", client::g_user_name, IM_ARRAYSIZE(client::g_user_name));

		if (ImGui::Button("Connect"))
		{
			client::start_ws();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		ImGui::Checkbox("Hide Country", &client::g_hide_client_country);

		ImGui::EndPopup();
	}
}

// Define the dockable windows for the application layout
inline std::vector<HelloImGui::DockableWindow> create_dockable_windows()
{
	// Create dockable windows  
	HelloImGui::DockableWindow logWindow;
	logWindow.label = "Log";
	logWindow.dockSpaceName = "LogSpace";
	logWindow.GuiFunction = []() { HelloImGui::LogGui(); };

	HelloImGui::DockableWindow chatWindow;
	chatWindow.label = "Chat";
	chatWindow.dockSpaceName = "MainDockSpace";
	chatWindow.GuiFunction = []() { chatbox_gui(); };
	chatWindow.rememberIsVisible = false;
	chatWindow.includeInViewMenu = false;
	chatWindow.canBeClosed = false;

	HelloImGui::DockableWindow userListWindow;
	userListWindow.label = "Users";
	userListWindow.dockSpaceName = "UserListSpace";
	userListWindow.GuiFunction = []() { user_list_gui(); };
	userListWindow.rememberIsVisible = false;
	userListWindow.includeInViewMenu = false;
	userListWindow.canBeClosed = false;

	return { logWindow, chatWindow, userListWindow };
}



int ShowAppMenuItems;
// Apply the application layout and windows to the runner parameters
inline void ApplyApplicationLayout(HelloImGui::RunnerParams* runnerParams)
{

	//Ini stuff
	runnerParams->iniFilename = "cvm_cpp_client.ini";

	//General Gui stuff to draw ontop of the interface
	runnerParams->callbacks.ShowGui = []() { settings_and_stuff_gui(); };

	// Define the application layout and windows
	runnerParams->appWindowParams.windowTitle = ui::g_window_title;
	runnerParams->imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

	// Add dockable windowsss
	runnerParams->dockingParams.dockingSplits = create_default_docking_splits();
	runnerParams->dockingParams.dockableWindows = create_dockable_windows();
	runnerParams->dockingParams.layoutCondition = HelloImGui::DockingLayoutCondition::ApplicationStart;

	//Add menu bar
	runnerParams->imGuiWindowParams.showMenuBar = true;
	runnerParams->imGuiWindowParams.showMenu_App = false;
	runnerParams->imGuiWindowParams.showMenu_View = false;
	runnerParams->imGuiWindowParams.menuAppTitle = "Main";

	// Inside `ShowMenus`, we can call `HelloImGui::ShowViewMenu` and `HelloImGui::ShowAppMenu` if desired
	runnerParams->callbacks.ShowMenus = [&runnerParams]() {menu_gui(*runnerParams); };

	//add items to Hello ImGui default App menu
	runnerParams->callbacks.ShowAppMenuItems = app_menu_gui;
}


int main(int, const char**)
{

#ifdef _WIN32 	// Required on Windows for websocket shit.
	ix::initNetSystem();
#endif

	// Init WS
	client::init_ws_handler();

	// Instantiate RunnerParams
	HelloImGui::RunnerParams runnerParams;

	// Apply the application layout configuration
	ApplyApplicationLayout(&runnerParams);

	// Run the ImGui application
	HelloImGui::Run(runnerParams);

#ifdef _WIN32
	ix::uninitNetSystem();
#endif
	return 0;
}
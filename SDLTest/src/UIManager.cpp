#include "UIManager.h"
#include "CustomStyles.h"
#include "imgui_impl_sdl3.h"

UIManager* UIManager::sharedInstance = nullptr;

void UIManager::initialize(SDL_Window* window)
{
	sharedInstance = new UIManager(window);
}

void UIManager::destroy()
{
	if (!sharedInstance->uiList.empty())
	{
		for (int i = sharedInstance->uiList.size() - 1; i >= 0; i--)
			delete sharedInstance->uiList[i];

		sharedInstance->uiTable.clear();
		sharedInstance->uiList.clear();
	}

	delete sharedInstance;
}

void UIManager::newFrame()
{
	//ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void UIManager::processEvent(const SDL_Event* event)
{
	ImGui_ImplSDL3_ProcessEvent(event);
}

void UIManager::drawAllUI()
{
	for (int i = 0; i < this->uiList.size(); i++)
	{
		if (this->uiList[i]->enabled)
			this->uiList[i]->DrawUI();
	}

	ImGui::Render();
}

UIScreen* UIManager::getUIScreen(std::string name)
{
	return this->uiTable[name];
}

bool UIManager::getEnabled(std::string name)
{
	UIScreen* ui = this->uiTable[name];
	return ui ? ui->enabled : false;
}

void UIManager::setEnabled(std::string name, bool enabled)
{
	UIScreen* ui = this->uiTable[name];
	if (ui) ui->enabled = enabled;
}

UIManager::UIManager(SDL_Window* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL3_InitForSDLGPU(window);
	
	CustomStyles styles;
	styles.StyleColorsCrimson();

	//UINames uiNames;
	//MenuScreen* menuScreen = new MenuScreen();
	//this->uiTable[uiNames.MENU_SCREEN] = menuScreen;
	//this->uiList.push_back(menuScreen);

}

UIManager::~UIManager()
{

}

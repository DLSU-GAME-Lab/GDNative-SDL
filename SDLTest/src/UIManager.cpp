#include "UIManager.h"
#include "CustomStyles.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
	return sharedInstance;
}

void UIManager::initialize(SDL_Window* window, SDL_Renderer* renderer)
{
	sharedInstance = new UIManager(window, renderer);
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

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	delete sharedInstance;
}

void UIManager::newFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void UIManager::processEvent(const SDL_Event* event)
{
	ImGui_ImplSDL3_ProcessEvent(event);
}

void UIManager::drawAllUI(SDL_Renderer* renderer)
{
	ImGui::ShowDemoWindow();
	for (int i = 0; i < this->uiList.size(); i++)
	{
		if (this->uiList[i]->enabled)
			this->uiList[i]->DrawUI();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

AUIScreen* UIManager::getUIScreen(std::string name)
{
	return this->uiTable[name];
}

bool UIManager::getEnabled(std::string name)
{
	AUIScreen* ui = this->uiTable[name];
	return ui ? ui->enabled : false;
}

void UIManager::setEnabled(std::string name, bool enabled)
{
	AUIScreen* ui = this->uiTable[name];
	if (ui) ui->enabled = enabled;
}

UIManager::UIManager(SDL_Window* window, SDL_Renderer* renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	CustomStyles styles;
	styles.StyleColorsCrimson();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
	
	//UINames uiNames;
	//MenuScreen* menuScreen = new MenuScreen();
	//this->uiTable[uiNames.MENU_SCREEN] = menuScreen;
	//this->uiList.push_back(menuScreen);

}

UIManager::~UIManager()
{

}

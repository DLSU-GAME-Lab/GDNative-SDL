#pragma once
#include "UINames.h"
#include "SDL3/SDL.h"

class UIManager
{
public:
	typedef std::vector<AUIScreen*> UIList;
	typedef std::unordered_map<std::string, AUIScreen*> UITable;

	static UIManager* getInstance();
	static void initialize(SDL_Window* window, SDL_Renderer* renderer);
	static void destroy();
	void newFrame();
	void processEvent(const SDL_Event* event);
	void drawAllUI(SDL_Renderer* renderer);

	AUIScreen* getUIScreen(std::string name);
	bool getEnabled(std::string name);
	void setEnabled(std::string name, bool enabled);

private:
	UIManager(SDL_Window* window, SDL_Renderer* renderer);
	~UIManager();
	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};

	static UIManager* sharedInstance;

	UIList uiList;
	UITable uiTable;
};
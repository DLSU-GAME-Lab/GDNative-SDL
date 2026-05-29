#pragma once
#include "ARenderer.h"
#include "Light2D.h"
#include <vector>

class Lightmap : public ARenderer
{
private:
	SDL_Texture* pLightmapTex = nullptr;
	SDL_Texture* pLightAccumTex = nullptr;
    SDL_Color color = {0, 0, 0, 255};
	std::vector<Light2D*> lights;
    bool bUpdateLightmap = true;
	bool bAccumulateLights = false;

public:
	void drawWidget() override;
	void perform() override;

	void addLight(Light2D* light);
	void removeLight(Light2D* light);
    void setAmbientColor(SDL_Color color);
    SDL_Color getAmbientColor() const;

private:
	void drawLightmap();


    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static Lightmap* P_SHARED_INSTANCE;

private:
    Lightmap();
    Lightmap(const Lightmap&) : ARenderer("Lightmap") {};
    Lightmap& operator = (const Lightmap&) {};

public:
    static void initialize();
    static void destroy();

    static Lightmap* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};


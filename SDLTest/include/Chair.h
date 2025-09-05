#include "AGameObject.h"
#include "SpriteRendererSystem.h"
class Chair : public AGameObject
{
private:
	bool bFlipX;
public:
	Chair(bool bFlipX);
	~Chair();

	void initialize() override;
};


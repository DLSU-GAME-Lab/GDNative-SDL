#include "AGameObject.h"
#include "SpriteRendererSystem.h"
class Prop : public AGameObject
{
private:
	std::string strImageName;
	bool bFlipX;
public:
	Prop(const std::string& strName, const std::string& strImageName, float nX, float nY, float nW, float nH, float fRot, bool bFlipX);
	~Prop();

	void initialize() override;
};

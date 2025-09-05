#include "AGameObject.h"
#include "SpriteRendererSystem.h"
class Prop : public AGameObject
{
private:
	std::string strImageName;
	int nX, nY, nH, nW;
	bool bFlipX;
public:
	Prop(const std::string& strName, const std::string& strImageName, int nX, int nY, int nW, int nH, bool bFlipX);
	~Prop();

	void initialize() override;
};

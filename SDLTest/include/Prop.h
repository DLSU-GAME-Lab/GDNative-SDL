#include "AGameObject.h"
#include "RenderSystem.h"
#include "PhysicsManager.h"
class Prop : public AGameObject
{
private:
	std::string strImageName;
	bool bFlipX;
	int dCount = 0;
public:
	Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX);
	~Prop();

	void initialize() override;


};

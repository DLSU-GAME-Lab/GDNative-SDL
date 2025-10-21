#include "AGameObject.h"

class Prop : public AGameObject
{
private:
	std::string strImageName;
	bool bFlipX;
	int dCount = 0;
public:
	Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate = Vector2D(), Vector2D fVecScale = Vector2D(), float fRot = 0.0f, bool bFlipX = false);

	void initialize() override;
};

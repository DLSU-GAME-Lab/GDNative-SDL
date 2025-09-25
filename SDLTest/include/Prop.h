#include "AGameObject.h"
#include "RenderSystem.h"
#include "PhysicsManager.h"
#include "ACollisionListener.hpp"
class Prop : public AGameObject, public ACollisionListener
{
private:
	std::string strImageName;
	bool bFlipX;
	int dCount = 0;
public:
	Prop(const std::string& strName, const std::string& strImageName, Vector2D fVecTranslate, Vector2D fVecScale, float fRot, bool bFlipX);
	~Prop();

	void initialize() override;

	// Inherited via ACollisionListener
	void onCollisionEnter(Collider* pCollider) override;
	void onCollisionContinue(Collider* pCollider) override;
	void onCollisionExit(Collider* pCollider) override;
};

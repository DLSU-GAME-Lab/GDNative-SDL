#pragma once
#include "AGameObject.h"
class PuzzleToken : public AGameObject
{
private:
	int nSlot;
	bool bDropped;

public:
	PuzzleToken(std::string strName, Vector2D position, Vector2D scale);

	void initialize() override;

	int getSlot() const;
	void setSlot(int nSlot);
	bool getDropped() const;
	void setDropped(bool bDropped);

};


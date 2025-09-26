#pragma once
#include "AGameObject.h"
#include "EnumGemType.h"

class Gem : public AGameObject
{
private:
	GemType EType;

public:
	Gem(std::string strName, GemType EType);
	~Gem();

	void initialize() override;

	GemType getType() const;
};


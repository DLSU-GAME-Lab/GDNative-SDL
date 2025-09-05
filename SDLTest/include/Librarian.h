#pragma once
#include "AGameObject.h"

class Librarian : public AGameObject
{
public:
	Librarian();
	~Librarian();

	void initialize() override;
};


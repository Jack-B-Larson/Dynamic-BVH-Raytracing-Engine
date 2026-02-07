#pragma once
#include "Component.h"
class Movement : public Component<Movement>
{
public:
	void Update() override;
};
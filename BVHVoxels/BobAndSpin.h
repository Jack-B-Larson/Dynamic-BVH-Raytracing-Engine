#pragma once
#include "Component.h"
class BobAndSpin : public Component<BobAndSpin>
{
public:
	void Update() override;
private:
	float progress;
};
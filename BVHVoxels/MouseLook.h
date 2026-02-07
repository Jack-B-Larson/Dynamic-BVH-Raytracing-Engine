#pragma once
#include "Component.h"
class MouseLook : public Component<MouseLook>
{
public:
	void Render() override;
private:
	float pitch = 0;
	float yaw = 0;
};
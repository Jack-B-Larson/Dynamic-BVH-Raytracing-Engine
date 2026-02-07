#pragma once
#include <glad/glad.h>
#include "Component.h"
class VoxelObject : public Component<VoxelObject>
{
public:
	void PreUpdate() override;
	void PostUpdate() override;
	void Render() override;
	void SetModelIndex(GLuint newModelIndex);
private:
	GLuint modelIndex = -1;
	GLuint objectIndex = -1;
};
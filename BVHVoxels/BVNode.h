#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
class BVNode
{
public:
	BVNode GetUnion(const BVNode& const other) const;
	void Union(const BVNode& const other);
	void CopyBounds(const BVNode& const other);
	bool GetIsLeafNode() const;
	float GetHalfSurfaceArea() const;

	glm::vec3 min = glm::vec3(0);
	GLuint left = -1;
	glm::vec3 max = glm::vec3(0);
	GLuint right = -1;
};


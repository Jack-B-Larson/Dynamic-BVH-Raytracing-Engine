#include "BVNode.h"

BVNode BVNode::GetUnion(const BVNode& const other) const
{
	BVNode bvNode = *this;
	bvNode.Union(other);
	return bvNode;
}

void BVNode::Union(const BVNode& const other)
{
	min = glm::min(min, other.min);
	max = glm::max(max, other.max);
}

void BVNode::CopyBounds(const BVNode& const other)
{
	min = other.min;
	max = other.max;
}

bool BVNode::GetIsLeafNode() const
{
	return right == -1;
}

float BVNode::GetHalfSurfaceArea() const
{
	glm::vec3 size = max - min;
	return size.x * size.y + size.y * size.z + size.z * size.x;
}

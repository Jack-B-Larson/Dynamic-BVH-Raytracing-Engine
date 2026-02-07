#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include "BVNode.h"
#include "SpanVector.h"
class BVHTree
{
public:
	void AddObject();
	void Insert(size_t objectIndex, const glm::vec3& const offset, const glm::ivec3& const size);
	void Remove(size_t objectIndex);

	void SetTransformationData(size_t objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation);


	void BufferNodes();
private:
	struct InsertionCost
	{
		size_t siblingIndex = -1;
		float inheritedCost = 0;
		float directCost = 0;

		bool operator<(const InsertionCost& const other) const
		{
			return (inheritedCost + directCost > other.inheritedCost + other.directCost);
		}
	};

	struct TransformationData
	{
		glm::mat4 transformation = glm::identity<glm::mat4>();
		glm::mat4 inverseTransformation = glm::identity<glm::mat4>();
	};

	size_t bvhBufferLength = 0;
	size_t transformationDatasBufferLength = 0;

	size_t GetBestSibling(const BVNode& const bounds);
	size_t GetSiblingNode(size_t nodeIndex);

	size_t rootNodeIndex = -1;

	SpanVector freeNodes = SpanVector();

	SpanVector changedNodes = SpanVector();
	SpanVector changedTransformations = SpanVector();

	std::vector<BVNode> nodes = std::vector<BVNode>();
	std::vector<size_t> nodeParents = std::vector<size_t>();

	std::vector<size_t> objectNodes = std::vector<size_t>();

	std::vector<TransformationData> transformationDatas = std::vector<TransformationData>();

	BVNode GetBounds(size_t objectIndex, const glm::vec3& const offset, const glm::ivec3& const size);
};


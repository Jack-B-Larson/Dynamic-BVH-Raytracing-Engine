#include <queue>
#include "BVHTree.h"
#include "GLFWHandler.h"
#include "GraphicsHandler.h"

void BVHTree::AddObject()
{
	objectNodes.push_back(-1);
	transformationDatas.push_back(TransformationData());
}

void BVHTree::Insert(size_t objectIndex, const glm::vec3& const offset, const glm::ivec3& const size)
{
	Remove(objectIndex);

	BVNode node = GetBounds(objectIndex, offset, size);

	if (rootNodeIndex == -1)
	{
		size_t nodeIndex = freeNodes.Pop(1);

		if (nodeIndex == -1)
		{
			nodeIndex = nodes.size();
			nodes.push_back(node);
			nodeParents.emplace_back(-1);
		}
		else
		{
			nodes[nodeIndex] = node;
			nodeParents[nodeIndex] = -1;
		}

		rootNodeIndex = nodeIndex;
		objectNodes[objectIndex] = nodeIndex;

		changedNodes.Push(nodeIndex, 1);

		return;
	}

	size_t parentIndex = GetBestSibling(node);

	size_t nodeIndex = freeNodes.Pop(1);

	if (nodeIndex == -1)
	{
		nodeIndex = nodes.size();
		nodes.push_back(node);
		nodeParents.emplace_back(parentIndex);
	}
	else
	{
		nodes[nodeIndex] = node;
		nodeParents[nodeIndex] = parentIndex;
	}

	objectNodes[objectIndex] = nodeIndex;

	changedNodes.Push(nodeIndex, 1);

	size_t siblingIndex = freeNodes.Pop(1);

	if (siblingIndex == -1)
	{
		siblingIndex = nodes.size();
		nodes.push_back(nodes[parentIndex]);
		nodeParents.emplace_back(parentIndex);
	}
	else
	{
		nodes[siblingIndex] = nodes[parentIndex];
		nodeParents[siblingIndex] = parentIndex;
	}

	if (nodes[siblingIndex].GetIsLeafNode())
	{
		objectNodes[nodes[siblingIndex].left] = siblingIndex;
	}
	else
	{
		nodeParents[nodes[siblingIndex].left] = siblingIndex;
		nodeParents[nodes[siblingIndex].right] = siblingIndex;
	}

	changedNodes.Push(siblingIndex, 1);

	nodes[parentIndex].left = nodeIndex;
	nodes[parentIndex].right = siblingIndex;

	size_t uncleIndex = GetSiblingNode(parentIndex);

	while (uncleIndex != -1)
	{
		float parentCostUnrotated = nodes[parentIndex].GetUnion(node).GetHalfSurfaceArea();
		float parentCostRotated = nodes[parentIndex].GetUnion(nodes[uncleIndex]).GetHalfSurfaceArea();

		if (parentCostRotated < parentCostUnrotated)
		{
			BVNode uncleNode = nodes[uncleIndex];
			nodes[uncleIndex] = node;
			nodes[nodeIndex] = uncleNode;

			if (uncleNode.GetIsLeafNode())
			{
				objectNodes[uncleNode.left] = nodeIndex;
			}
			else
			{
				nodeParents[uncleNode.left] = nodeIndex;
				nodeParents[uncleNode.right] = nodeIndex;
			}

			if (node.GetIsLeafNode())
			{
				objectNodes[node.left] = uncleIndex;
			}
			else
			{
				nodeParents[node.left] = uncleIndex;
				nodeParents[node.right] = uncleIndex;
			}

			nodeIndex = uncleIndex;

			changedNodes.Push(uncleIndex, 1);

			nodes[parentIndex].Union(nodes[uncleIndex]);
		}
		else
		{
			nodes[parentIndex].Union(nodes[nodeIndex]);

			nodeIndex = nodeParents[nodeIndex];
		}

		changedNodes.Push(parentIndex, 1);

		node = nodes[nodeIndex];
		parentIndex = nodeParents[nodeIndex];
		uncleIndex = GetSiblingNode(parentIndex);
	}

	nodes[parentIndex].Union(nodes[nodeIndex]);

	changedNodes.Push(parentIndex, 1);

}

void BVHTree::Remove(size_t objectIndex)
{

	size_t nodeIndex = objectNodes[objectIndex];

	if (nodeIndex == -1)
	{
		return;
	}

	freeNodes.Push(nodeIndex, 1);

	if (nodeIndex == rootNodeIndex)
	{
		rootNodeIndex = -1;
		return;
	}

	size_t parentIndex = nodeParents[nodeIndex];


	objectNodes[objectIndex] = -1;

	freeNodes.Push(parentIndex, 1);

	size_t siblingIndex = GetSiblingNode(nodeIndex);

	if (parentIndex == rootNodeIndex)
	{
		nodeParents[siblingIndex] = -1;
		rootNodeIndex = siblingIndex;
		return;
	}

	size_t grandParentIndex = nodeParents[parentIndex];

	size_t uncleIndex = GetSiblingNode(parentIndex);

	nodes[grandParentIndex].left = uncleIndex;
	nodes[grandParentIndex].right = siblingIndex;

	nodeParents[siblingIndex] = grandParentIndex;



	size_t ancestorIndex = grandParentIndex;

	while (ancestorIndex != -1)
	{
		nodes[ancestorIndex].CopyBounds(nodes[nodes[ancestorIndex].left]);
		nodes[ancestorIndex].Union(nodes[nodes[ancestorIndex].right]);

		changedNodes.Push(ancestorIndex, 1);

		ancestorIndex = nodeParents[ancestorIndex];
	}
}

void BVHTree::SetTransformationData(size_t objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation)
{
	transformationDatas[objectIndex].transformation = transformation;
	transformationDatas[objectIndex].inverseTransformation = inverseTransformation;

	changedTransformations.Push(objectIndex, 1);
}

void BVHTree::BufferNodes()
{
	if (bvhBufferLength != nodes.size())
	{
		bvhBufferLength = nodes.size();

		GLFWHandler::ReallocateBuffer(GraphicsHandler::GetBVHBuffer(), nodes.size() * sizeof(BVNode), nodes.data());
	}
	else
	{
		for (std::pair<size_t, size_t> changedSpan : changedNodes.GetSpans())
		{
			GLFWHandler::BufferData(GraphicsHandler::GetBVHBuffer(), changedSpan.first * sizeof(BVNode), changedSpan.second * sizeof(BVNode), &nodes[changedSpan.first]);
		}
		changedNodes.Clear();
	}

	if (transformationDatasBufferLength != transformationDatas.size())
	{
		transformationDatasBufferLength = transformationDatas.size();

		GLFWHandler::ReallocateBuffer(GraphicsHandler::GetTransformationDatasBuffer(), transformationDatas.size() * sizeof(TransformationData), transformationDatas.data());
	}
	else
	{
		for (std::pair<size_t, size_t> changedSpan : changedTransformations.GetSpans())
		{
			GLFWHandler::BufferData(GraphicsHandler::GetTransformationDatasBuffer(), changedSpan.first * sizeof(TransformationData), changedSpan.second * sizeof(TransformationData), &transformationDatas[changedSpan.first]);
		}
		changedTransformations.Clear();
	}

	GraphicsHandler::SetBVRootNodeIndex(GLuint(rootNodeIndex));




	//	Logger::LogInstant("renderend" + std::to_string(rootNodeIndex));
	//	for (int i = 0; i < objectNodes.size(); i++)
	//	{
	//		std::string output = " " + std::to_string(i) + " : (" + std::to_string(objectNodes[i]) + ")";
	//		if (freeNodes.Contains(objectNodes[i], 1))
	//		{
	//			output = " ~" + output;
	//		}
	//		Logger::LogInstant(output);
	//	}
	//	Logger::LogInstant("");
	//	for (int i = 0; i < nodes.size(); i++)
	//	{
	//		std::string output = " " + std::to_string(i) + " : (" + std::to_string(nodes[i].left) + ", " + std::to_string(nodes[i].right) + ") ^ " + std::to_string(nodeParents[i]);
	//		if (freeNodes.Contains(i, 1))
	//		{
	//			output = " ~" + output;
	//		}
	//		Logger::LogInstant(output);
	//	
	//		output = " " + std::to_string(i) + " : (" + std::to_string(nodes[i].min.x) + ", " + std::to_string(nodes[i].min.y) + ", " + std::to_string(nodes[i].min.z) + ") - (" + std::to_string(nodes[i].max.x) + ", " + std::to_string(nodes[i].max.y) + ", " + std::to_string(nodes[i].max.z) + ")";
	//		if (freeNodes.Contains(i, 1))
	//		{
	//			output = " ~" + output;
	//		}
	//		Logger::LogInstant(output);
	//	}
	//	
	//	for (int i = 0; i < nodes.size(); i++)
	//	{
	//		if (freeNodes.Contains(i, 1))
	//		{
	//			continue;
	//		}
	//		if (nodes[i].GetIsLeafNode())
	//		{
	//			continue;
	//		}
	//		if (glm::max(nodes[nodes[i].left].max, nodes[nodes[i].right].max).length() > nodes[i].max.length())
	//		{
	//			Logger::LogInstant(std::to_string(i));
	//			throw;
	//		}
	//		if (glm::min(nodes[nodes[i].left].min, nodes[nodes[i].right].min).length() != nodes[i].min.length())
	//		{
	//			Logger::LogInstant(std::to_string(i));
	//			throw;
	//		}
	//	}
}

size_t BVHTree::GetBestSibling(const BVNode& const bounds)
{
	std::priority_queue<InsertionCost> potentialSiblings = std::priority_queue<InsertionCost>();

	{
		InsertionCost rootNodeCost = InsertionCost();
		rootNodeCost.siblingIndex = rootNodeIndex;
		rootNodeCost.directCost = nodes[rootNodeCost.siblingIndex].GetUnion(bounds).GetHalfSurfaceArea();

		potentialSiblings.push(rootNodeCost);
	}

	float objectCost = bounds.GetHalfSurfaceArea();

	size_t bestSibling = -1;

	while (bestSibling != potentialSiblings.top().siblingIndex)
	{
		bestSibling = potentialSiblings.top().siblingIndex;

		if (nodes[bestSibling].GetIsLeafNode())
		{
			continue;
		}

		float inheritedCost = potentialSiblings.top().inheritedCost + nodes[bestSibling].GetUnion(bounds).GetHalfSurfaceArea() - nodes[bestSibling].GetHalfSurfaceArea();

		float idealCost = inheritedCost + objectCost;

		if (idealCost < potentialSiblings.top().inheritedCost + potentialSiblings.top().directCost)
		{
			InsertionCost nodeCost = InsertionCost();
			nodeCost.siblingIndex = nodes[bestSibling].left;
			nodeCost.inheritedCost = inheritedCost;
			nodeCost.directCost = nodes[nodeCost.siblingIndex].GetUnion(bounds).GetHalfSurfaceArea();

			potentialSiblings.push(nodeCost);

			nodeCost.siblingIndex = nodes[bestSibling].right;
			nodeCost.inheritedCost = inheritedCost;
			nodeCost.directCost = nodes[nodeCost.siblingIndex].GetUnion(bounds).GetHalfSurfaceArea();

			potentialSiblings.emplace(std::move(nodeCost));
		}
	}

	return bestSibling;
}

size_t BVHTree::GetSiblingNode(size_t nodeIndex)
{
	if (nodeParents[nodeIndex] == -1)
	{
		return -1;
	}
	const BVNode& const parentNode = nodes[nodeParents[nodeIndex]];
	if (parentNode.left == nodeIndex)
	{
		return parentNode.right;
	}
	return parentNode.left;
}

BVNode BVHTree::GetBounds(size_t objectIndex, const glm::vec3& const offset, const glm::ivec3& const size)
{
	BVNode bounds = BVNode();

	glm::mat4 transform = transformationDatas[objectIndex].transformation;
	glm::vec3 extent = glm::vec3(size) * 0.5f;
	glm::vec3 worldOffset = transform * glm::vec4(offset + extent, 1);
	transform[0] = glm::abs(transform[0]);
	transform[1] = glm::abs(transform[1]);
	transform[2] = glm::abs(transform[2]);
	transform[3] = glm::abs(transform[3]);
	extent = transform * glm::vec4(extent, 0);
	bounds.min = worldOffset - extent;
	bounds.max = worldOffset + extent;

	bounds.left = objectIndex;

	return bounds;
}

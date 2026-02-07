#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <utility>
#include "BVH.h"
#include "BVNode.h"
#include "GLFWHandler.h"
#include "GraphicsHandler.h"

void BVH::SetOffset(GLuint modelIndex, const glm::vec3& offset)
{
	modelDatas[modelIndex].offset = offset;
}

GLuint BVH::RegisterModelData(const glm::ivec3& size)
{
	bool foundSlot = false;
	GLuint flatSize = size.x * size.y * size.z;

	size_t dataIndex = freeDatas.Pop(flatSize);

	if (dataIndex == -1)
	{
		dataIndex = voxelData.size();
		voxelData.resize(dataIndex + flatSize);

		changedVoxelData.Push(dataIndex, flatSize);
	}
	size_t paletteIndex = freePalettes.Pop(0);
	if (paletteIndex == -1)
	{
		paletteIndex = paletteDatas.size();
	}

	GLuint modelIndex = freeModelIndices.Pop(1);

	if (modelIndex == -1)
	{
		modelIndex = modelDatas.size();
		modelDatas.push_back(ModelData());
		modelReferenceCount.push_back(0);
		paletteSizes.push_back(0);
	}

	modelDatas[modelIndex].size = size;
	modelDatas[modelIndex].dataIndex = dataIndex;
	modelDatas[modelIndex].offset = glm::vec3(0);
	modelDatas[modelIndex].paletteIndex = paletteIndex;
	modelReferenceCount[modelIndex] = 0;
	paletteSizes[modelIndex] = 0;

	changedModelDatas.Push(modelIndex, 1);
	return modelIndex;
}

void BVH::DeRegisterModelData(GLuint modelIndex)
{
	GLuint flatSize = modelDatas[modelIndex].size.x * modelDatas[modelIndex].size.y * modelDatas[modelIndex].size.z;
	freeModelIndices.Push(modelIndex, 1);
	freeDatas.Push(modelDatas[modelIndex].dataIndex, flatSize);
	freePalettes.Push(modelDatas[modelIndex].paletteIndex, paletteSizes[modelIndex]);
}

void BVH::Render()
{
	if (objectModelsBufferLength != objectModels.size())
	{
		objectModelsBufferLength = objectModels.size();

		GLFWHandler::ReallocateBuffer(GraphicsHandler::GetObjectModelsBuffer(), objectModels.size() * sizeof(GLuint), objectModels.data());

	}
	else
	{
		for (std::pair<size_t, size_t> changedSpan : changedObjects.GetSpans())
		{
			GLFWHandler::BufferData(GraphicsHandler::GetObjectModelsBuffer(), changedSpan.first * sizeof(GLuint), changedSpan.second * sizeof(GLuint), &objectModels[changedSpan.first]);
		}
		changedObjects.Clear();
	}
	if (modelDatasBufferLength != modelDatas.size())
	{
		modelDatasBufferLength = modelDatas.size();

		GLFWHandler::ReallocateBuffer(GraphicsHandler::GetModelDatasBuffer(), modelDatas.size() * sizeof(ModelData), modelDatas.data());
	}
	else
	{
		for (std::pair<size_t, size_t> changedSpan : changedModelDatas.GetSpans())
		{
			GLFWHandler::BufferData(GraphicsHandler::GetModelDatasBuffer(), changedSpan.first * sizeof(ModelData), changedSpan.second * sizeof(ModelData), &modelDatas[changedSpan.first]);
		}
		changedModelDatas.Clear();
	}

	renderBVHTree.BufferNodes();
}

GLuint BVH::RegisterObject(GLuint modelIndex)
{
	GLuint objectIndex = freeObjectIndices.Pop(1);
	if (objectIndex == -1)
	{
		objectIndex = objectModels.size();
		objectModels.push_back(-1);

		renderBVHTree.AddObject();
	}

	SetObjectModel(objectIndex, modelIndex);
	SetTransformationData(objectIndex, glm::identity<glm::mat4>(), glm::identity<glm::mat4>());
	SetRenderTransformationData(objectIndex, glm::identity<glm::mat4>(), glm::identity<glm::mat4>());


	changedObjects.Push(objectIndex, 1);

	return objectIndex;
}

void BVH::DeRegisterObject(GLuint objectIndex)
{
	SetObjectModel(objectIndex, -1);
	freeObjectIndices.Push(objectIndex, 1);
}

void BVH::SetObjectModel(GLuint objectIndex, GLuint modelIndex)
{
	GLuint oldModel = objectModels[objectIndex];
	if (oldModel != -1)
	{
		modelReferenceCount[oldModel] -= 1;
		if (modelReferenceCount[oldModel] == 0)
		{
			DeRegisterModelData(oldModel);
		}
	}
	objectModels[objectIndex] = modelIndex;
	if (modelIndex != -1)
	{
		modelReferenceCount[modelIndex] += 1;
	}
}

void BVH::SetTransformationData(GLuint objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation)
{
}

void BVH::SetRenderTransformationData(GLuint objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation)
{
	renderBVHTree.SetTransformationData(objectIndex, transformation, inverseTransformation);

	const ModelData& const modelData = modelDatas[objectModels[objectIndex]];

	renderBVHTree.Insert(objectIndex, modelData.offset, modelData.size);
}

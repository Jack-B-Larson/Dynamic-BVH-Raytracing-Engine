#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "BVHTree.h"
#include "SpanVector.h"
#include "VoxelData.h"
class BVH
{
public:
	void SetOffset(GLuint modelIndex, const glm::vec3& offset);
	GLuint RegisterModelData(const glm::ivec3& size);
	void DeRegisterModelData(GLuint modelIndex);

	void Render();

	GLuint RegisterObject(GLuint modelIndex);
	void DeRegisterObject(GLuint objectIndex);

	void SetObjectModel(GLuint objectIndex, GLuint modelIndex);

	void SetTransformationData(GLuint objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation);
	void SetRenderTransformationData(GLuint objectIndex, const glm::mat4& transformation, const glm::mat4& inverseTransformation);

private:

	struct ModelData
	{
		glm::vec3 offset = glm::vec3(0);
		GLuint paletteIndex = -1;
		glm::ivec3 size = glm::ivec3(0);
		GLuint dataIndex = -1;
	};

	size_t objectModelsBufferLength = 0;
	size_t modelDatasBufferLength = 0;


	BVHTree bvhTree = BVHTree();
	BVHTree renderBVHTree = BVHTree();

	SpanVector changedObjects = SpanVector();
	SpanVector changedModelDatas = SpanVector();
	SpanVector changedVoxelData = SpanVector();
	SpanVector changedPaletteData = SpanVector();


	SpanVector freeObjectIndices = SpanVector();

	std::vector<size_t> modelReferenceCount = std::vector<size_t>();

	std::vector<GLuint> objectModels = std::vector<GLuint>();

	SpanVector freeModelIndices = SpanVector();
	SpanVector freeDatas = SpanVector();
	SpanVector freePalettes = SpanVector();


	std::vector<ModelData> modelDatas = std::vector<ModelData>();
	std::vector<GLuint> paletteSizes = std::vector<GLuint>();
	std::vector<unsigned char> voxelData = std::vector<unsigned char>();
	std::vector<VoxelData> paletteDatas = std::vector<VoxelData>();

};


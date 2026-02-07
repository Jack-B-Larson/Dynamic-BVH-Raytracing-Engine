uniform ivec2 resolution;
uniform float resolution_aspect_ratio;
uniform float window_aspect_ratio;

uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;

uniform uint bvRootNodeIndex;

layout(rgba8, binding = 0) uniform image2D colorTexture;


layout(std430, binding = 0) buffer bvhBuffer
{
	BVNode bvNodes[];
};
layout(std430, binding = 1) buffer transformationDatasBuffer
{
	TransformationData transformationDatas[];
};
layout(std430, binding = 2) buffer objectModelsBuffer
{
	uint objectModels[];
};
layout(std430, binding = 3) buffer modelDatasBuffer
{
	ModelData modelDatas[];
};
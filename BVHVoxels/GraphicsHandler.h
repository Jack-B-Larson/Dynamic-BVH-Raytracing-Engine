#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ThreadSafeQueue.h"
namespace GraphicsHandler
{
	void Initialize();
	void Destroy();

	void CreateMainWindow();
	void DestroyMainWindow();

	void CreateShaders();
	void DestroyShaders();

	void CreateTextures();
	void DestroyTextures();

	void CreateBuffers();
	void DestroyBuffers();

	void RecalculateAspectRatio();
	void AspectRatioVariableRefresh();
	void RecalculateResolutionAspectRatio();
	void ResolutionAspectRatioVariableRefresh();
	void WindowSizeVariableCallback();
	void WindowSizeVariableRefresh();
	void WindowTitleVariableCallback();
	void WindowFullscreenVariableCallback();
	void ResolutionSizeVariableCallback();
	void ResolutionSizeVariableRefresh();

	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	void WindowCloseCallback(GLFWwindow* window);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
	void MousePositionCallback(GLFWwindow* window, double x, double y);

	void UpdateLoop();
	void Render();
	void RenderLoop();

	void SetCamera(Camera* camera);
	void SetBVRootNodeIndex(GLuint index);

	bool GetInitialized();

	GLuint GetBVHBuffer();
	GLuint GetTransformationDatasBuffer();
	GLuint GetObjectModelsBuffer();
	GLuint GetModelDatasBuffer();

	namespace
	{
		bool initialized = false;

		ThreadSafeQueue<void(*)()> variableRefreshQueue = ThreadSafeQueue<void(*)()>();

		GLFWwindow* mainWindow = nullptr;

		GLuint screenQuadShader = -1;
		GLuint voxelWorldShader = -1;

		GLuint bvhBuffer = -1;
		GLuint transformationDatasBuffer = -1;
		GLuint objectModelsBuffer = -1;
		GLuint modelDatasBuffer = -1;
		GLuint voxelDataBuffer = -1;
		GLuint voxelPaletteBuffer = -1;

		GLuint colorTexture = -1;
	}
}
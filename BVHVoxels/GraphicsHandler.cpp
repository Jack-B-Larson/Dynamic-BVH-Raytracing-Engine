#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "EngineVariables.h"
#include "GameHandler.h"
#include "GameObject.h"
#include "GameTimeHandler.h"
#include "GLFWHandler.h"
#include "GraphicsHandler.h"
#include "InputHandler.h"
#include "Logger.h"
#include "Program.h"
#include "Transform.h"
namespace GraphicsHandler
{
	void Initialize()
	{
		//create window
		CreateMainWindow();

		//create shaders
		CreateShaders();

		//create buffers
		CreateBuffers();

		//create textures
		CreateTextures();

		glfwMakeContextCurrent(NULL);
	}
	void Destroy()
	{
		//delete window
		DestroyMainWindow();

		//delete shaders
		DestroyShaders();

		//delete buffers
		DestroyBuffers();

		//delete textures
		DestroyTextures();

	}
	void CreateMainWindow()
	{
		//destroy old window if exists
		if (mainWindow != NULL)
		{
			DestroyMainWindow();
		}

		//create window by engine variables
		mainWindow = GLFWHandler::CreateWindow(
			EngineVariables::GetVariableInt("window_width"),
			EngineVariables::GetVariableInt("window_height"),
			EngineVariables::GetVariableString("window_title"),
			NULL, NULL);

		//make sure we match set variables for fullscreen
		WindowFullscreenVariableCallback();

		//set size callback
		glfwSetFramebufferSizeCallback(mainWindow, FrameBufferSizeCallback);

		//set close callback
		glfwSetWindowCloseCallback(mainWindow, WindowCloseCallback);

		//set key callback
		glfwSetKeyCallback(mainWindow, KeyCallback);
		//set mouse position callback
		glfwSetCursorPosCallback(mainWindow, MousePositionCallback);
	}
	void DestroyMainWindow()
	{
		//skip if window doesn't exist 
		if (mainWindow == NULL)
		{
			Logger::Log("window already destroyed");
			return;
		}

		//destroy window, buffers, and reset pointer
		GLFWHandler::DestroyWindow(mainWindow);
		mainWindow = NULL;

	}

	void CreateShaders()
	{
		//create shaders
		screenQuadShader = GLFWHandler::CreateShaderFromFile("Content\\Shaders\\ScreenQuad");
		voxelWorldShader = GLFWHandler::CreateShaderFromFile("Content\\Shaders\\VoxelTracer");

		//set defaults
		GLFWHandler::SetShaderUInt(screenQuadShader, "textureSampler", colorTexture);

		GLFWHandler::SetShaderIVec2(voxelWorldShader, "resolution", EngineVariables::GetVariableInt("resolution_width"), EngineVariables::GetVariableInt("resolution_height"));
		GLFWHandler::SetShaderFloat(voxelWorldShader, "window_aspect_ratio", EngineVariables::GetVariableFloat("window_aspect_ratio"));
		GLFWHandler::SetShaderFloat(voxelWorldShader, "resolution_aspect_ratio", EngineVariables::GetVariableFloat("resolution_aspect_ratio"));

	}

	void DestroyShaders()
	{
		//delete shaders
		GLFWHandler::DestroyShader(screenQuadShader);
		GLFWHandler::DestroyShader(voxelWorldShader);
	}

	void CreateTextures()
	{
		//create textures
		colorTexture = GLFWHandler::CreateTexture(EngineVariables::GetVariableInt("resolution_width"), EngineVariables::GetVariableInt("resolution_height"));
	}

	void DestroyTextures()
	{
		//destroy textures
		GLFWHandler::DestroyTexture(colorTexture);
	}

	void CreateBuffers()
	{
		//create basic buffers for raster graphics (fullscreen quad)
		GLFWHandler::CreateBuffers();

		constexpr float vertexData[] = {
			1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f
		};
		constexpr GLuint elementData[] = {
			0, 1, 2,
			3, 0, 2
		};

		//fill basic buffers with screen space quad
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementData), elementData, GL_STATIC_DRAW);

		glVertexAttribPointer(GLFWHandler::GetAttributeLocation(screenQuadShader, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(GLFWHandler::GetAttributeLocation(screenQuadShader, "vertexUV"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);



		//create buffers
		bvhBuffer = GLFWHandler::CreateBuffer(0);

		transformationDatasBuffer = GLFWHandler::CreateBuffer(1);

		objectModelsBuffer = GLFWHandler::CreateBuffer(2);

		modelDatasBuffer = GLFWHandler::CreateBuffer(3);
	}

	void DestroyBuffers()
	{
		//destroy basic buffers
		GLFWHandler::DestroyBuffers();

		//destroy buffers
		GLFWHandler::DestroyBuffer(bvhBuffer);

		GLFWHandler::DestroyBuffer(transformationDatasBuffer);

		GLFWHandler::DestroyBuffer(objectModelsBuffer);

		GLFWHandler::DestroyBuffer(modelDatasBuffer);
	}

	void RecalculateAspectRatio()
	{
		//set "window_aspect_ratio" to "window_width"/"window_height"
		EngineVariables::SetVariableFloat("window_aspect_ratio", EngineVariables::GetVariableFloat("window_width") / EngineVariables::GetVariableFloat("window_height"));


		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//queue render thread side update
		variableRefreshQueue.push(AspectRatioVariableRefresh);
	}

	void AspectRatioVariableRefresh()
	{
		//update shader uniform
		GLFWHandler::SetShaderFloat(voxelWorldShader, "window_aspect_ratio", EngineVariables::GetVariableFloat("window_aspect_ratio"));
	}

	void RecalculateResolutionAspectRatio()
	{
		//set "resolution_aspect_ratio" to "resolution_width"/"resolution_height"
		EngineVariables::SetVariableFloat("resolution_aspect_ratio", EngineVariables::GetVariableFloat("resolution_width") / EngineVariables::GetVariableFloat("resolution_height"));


		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//queue render thread side update
		variableRefreshQueue.push(ResolutionAspectRatioVariableRefresh);
	}

	void ResolutionAspectRatioVariableRefresh()
	{
		//update shader uniform
		GLFWHandler::SetShaderFloat(voxelWorldShader, "resolution_aspect_ratio", EngineVariables::GetVariableFloat("resolution_aspect_ratio"));
	}

	void WindowSizeVariableCallback()
	{
		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//update window size
		glfwSetWindowSize(
			mainWindow,
			EngineVariables::GetVariableInt("window_width"),
			EngineVariables::GetVariableInt("window_height"));

		//queue render thread side update
		variableRefreshQueue.push(WindowSizeVariableRefresh);
	}

	void WindowSizeVariableRefresh()
	{
		//update viewport
		glViewport(0, 0, EngineVariables::GetVariableInt("window_width"), EngineVariables::GetVariableInt("window_height"));
	}

	void WindowTitleVariableCallback()
	{
		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//update window title
		glfwSetWindowTitle(mainWindow, EngineVariables::GetVariableString("window_title").c_str());
	}

	void WindowFullscreenVariableCallback()
	{
		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//update window fullscreen status
		GLFWHandler::UpdateWindowFullscreen(mainWindow, EngineVariables::GetVariableBool("window_fullscreen"));

		//update window size variables manually
		int width;
		int height;
		glfwGetFramebufferSize(mainWindow, &width, &height);
		EngineVariables::SetVariableInt("window_width", width);
		EngineVariables::SetVariableInt("window_height", height);
	}

	void ResolutionSizeVariableCallback()
	{
		//skip if window not created
		if (mainWindow == NULL)
		{
			return;
		}

		//recreate textures
		DestroyTextures();
		CreateTextures();


		//queue render thread side update
		variableRefreshQueue.push(ResolutionSizeVariableRefresh);
	}

	void ResolutionSizeVariableRefresh()
	{
		//update shader uniform
		GLFWHandler::SetShaderIVec2(voxelWorldShader, "resolution", EngineVariables::GetVariableInt("resolution_width"), EngineVariables::GetVariableInt("resolution_height"));
	}


	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		//update engine variables
		EngineVariables::SetVariableInt("window_width", width);
		EngineVariables::SetVariableInt("window_height", height);
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		//request shutdown
		Program::Exit();
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
	{
		if (action == GLFW_REPEAT)
		{
			return;
		}
		InputHandler::UpdateKeyStatus(key, action);
	}

	void MousePositionCallback(GLFWwindow* window, double x, double y)
	{
		InputHandler::UpdateMousePosition(x, y);
	}

	void UpdateLoop()
	{
		//loop update while window isn't closing
		while (!Program::GetShouldExit())
		{
			//wait events
			glfwWaitEvents();
		}
	}

	void Render()
	{
		//use compute shader
		GLFWHandler::UseShader(voxelWorldShader);

		//run render update
		GameHandler::Render();

		//render to texture
		glDispatchCompute((EngineVariables::GetVariableInt("resolution_width") + 31) / 32, (EngineVariables::GetVariableInt("resolution_height") + 31) / 32, 1);

		//wait for texture
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		//use quad shader
		GLFWHandler::UseShader(screenQuadShader);

		//set texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture);

		//render quad
		GLFWHandler::RenderElements(mainWindow);
	}

	void RenderLoop()
	{
		//steal window context
		glfwMakeContextCurrent(mainWindow);

		initialized = true;

		while (!Program::GetInitialized())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		//loop render while window isn't closing
		while (!Program::GetShouldExit())
		{
			//render if allowed by target render time
			if (GameTimeHandler::ShouldRender())
			{
				//update variables needed on render thread
				while (!variableRefreshQueue.empty())
				{
					variableRefreshQueue.pop()();
				}

				//render
				Render();
			}
		}
	}
	void SetCamera(Camera* camera)
	{
		//set shader projections
		GLFWHandler::SetShaderMat4(voxelWorldShader, "projectionMatrix", camera->GetProjection());
		GLFWHandler::SetShaderMat4(voxelWorldShader, "inverseProjectionMatrix", camera->GetInverseProjection());
		//set shader views
		GLFWHandler::SetShaderMat4(voxelWorldShader, "viewMatrix", camera->GetOwner()->GetComponentOfType<Transform>()->GetInterpolatedTransformation());
		GLFWHandler::SetShaderMat4(voxelWorldShader, "inverseViewMatrix", camera->GetOwner()->GetComponentOfType<Transform>()->GetInterpolatedInverseTransformation());
	}
	void SetBVRootNodeIndex(GLuint index)
	{
		//set bvh root node index uniform
		GLFWHandler::SetShaderUInt(voxelWorldShader, "bvRootNodeIndex", index);
	}
	bool GetInitialized()
	{
		return initialized;
	}
	GLuint GetBVHBuffer()
	{
		return bvhBuffer;
	}
	GLuint GetTransformationDatasBuffer()
	{
		return transformationDatasBuffer;
	}
	GLuint GetObjectModelsBuffer()
	{
		return objectModelsBuffer;
	}
	GLuint GetModelDatasBuffer()
	{
		return modelDatasBuffer;
	}
}
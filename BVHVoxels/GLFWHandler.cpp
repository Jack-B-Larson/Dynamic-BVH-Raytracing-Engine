#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "EngineVariables.h"
#include "FileSystemHandler.h"
#include "GLFWHandler.h"
#include "Logger.h"
namespace GLFWHandler
{
	void Initialize()
	{
		//skip if already initialized
		if (isInitialized)
		{
			Logger::Log("glfw already initialized");
			return;
		}

		//attempt initialization
		isInitialized = glfwInit();

		//error if failed
		if (!isInitialized)
		{
			Logger::Error("glfw initialization failed");
		}
	}

	void Terminate()
	{
		//skip if already terminated
		if (!isInitialized)
		{
			Logger::Log("glfw already terminated");
			return;
		}

		//terminate
		glfwTerminate();
		isInitialized = false;
	}

	GLFWwindow* CreateWindow(int width, int height, std::string const& title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		Initialize();
		//set glfw version to assigned engine settings
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, EngineVariables::GetVariableInt("glfw_context_version") / 10);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, EngineVariables::GetVariableInt("glfw_context_version") % 10);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//set to sdr rendering, hdr may get support if memory is free
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 0);
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_STENCIL_BITS, 0);

		//attempt to create window
		GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, share);

		//catch window creation failure
		if (window == NULL)
		{
			glfwTerminate();
			Logger::Error("glfw window creation failed");
		}

		//attempt to link context to window
		glfwMakeContextCurrent(window);

		//catch glad initialization error
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			Logger::Error("glad initialization failed");
		}

		if (glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		//set viewport to window size
		glViewport(0, 0, EngineVariables::GetVariableInt("window_width"), EngineVariables::GetVariableInt("window_height"));

		glfwSwapInterval(EngineVariables::GetVariableInt("glfw_swap_interval"));

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		return window;
	}
	void DestroyWindow(GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	}

	void UpdateWindowFullscreen(GLFWwindow* window, bool fullscreen)
	{
		//update window border mode
		glfwSetWindowAttrib(window, GLFW_DECORATED, !fullscreen);


		if (fullscreen)
		{
			//if fullscreen, maximize
			glfwMaximizeWindow(window);
		}
		else
		{
			//if not fullscreen, restore
			glfwRestoreWindow(window);
		}
	}
	void CreateBuffers()
	{
		//generate empty VAO, VBO, and EBO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	void DestroyBuffers()
	{
		//destroy VAO, VBO, EBO
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	void RenderElements(GLFWwindow* window)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}
	double GetTime()
	{
		//return time
		return glfwGetTime();
	}
	GLuint CreateShaderFromFile(const std::string const& path)
	{
		//create program
		GLuint programID = glCreateProgram();


		//loop through found files
		std::vector<GLuint> shaderIDs;
		for (const std::string& file : FileSystemHandler::GetFilesInDirectory(FileSystemHandler::GetDirectory(path), false))
		{
			//skip file if it doesn't match shader name
			if (FileSystemHandler::GetStem(file) != FileSystemHandler::GetStem(path))
			{
				continue;
			}
			//set compiler type by file extension
			GLenum type = 0;
			std::string extension = FileSystemHandler::GetExtension(file);
			if (extension == ".vert")
			{
				type = GL_VERTEX_SHADER;
			}
			if (extension == ".frag")
			{
				type = GL_FRAGMENT_SHADER;
			}
			if (extension == ".comp")
			{
				type = GL_COMPUTE_SHADER;
			}


			//skip if non-shader file
			if (type == 0)
			{
				continue;
			}

			//load file
			std::string source = FileSystemHandler::LoadPlainText(file);



			//inline include files as requested
			size_t includeIndex = source.find("#include \"");

			while (includeIndex != std::string::npos)
			{
				size_t preSplit = includeIndex;
				size_t postSplit = source.find("\n", includeIndex);

				source = source.substr(0, preSplit) + FileSystemHandler::LoadPlainText("Content\\Shaders\\" + source.substr(preSplit + 10, postSplit - preSplit - 11)) + source.substr(postSplit);

				includeIndex = source.find("#include \"");
			}


			//compile shader from source and attach to program
			GLuint shaderID;
			CompileShader(shaderID, source, type);
			shaderIDs.push_back(shaderID);

			glAttachShader(programID, shaderID);
		}

		//throw an error if no shader files were found
		if (shaderIDs.empty())
		{
			Logger::Error("failed to load shader \"" + path + "\"");
		}

		//link program after collecting compiled shaders
		glLinkProgram(programID);

		//if errored, log
		GLint success;
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetProgramInfoLog(programID, 1024, NULL, infoLog);
			Logger::Error("program failed to link \"" + std::string(infoLog) + "\"");
		}

		//destroy shaders as they are no longer required
		for (GLuint shaderID : shaderIDs)
		{
			glDeleteShader(shaderID);
		}

		Logger::Log("loaded shader \"" + path + "\"");
		//return
		return programID;
	}
	void CompileShader(GLuint& id, const std::string const& source, GLenum type)
	{
		//compile shader from source and attach to id
		const GLchar* charSource = source.c_str();
		id = glCreateShader(type);
		glShaderSource(id, 1, &charSource, NULL);
		glCompileShader(id);


		//if errored, log
		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetShaderInfoLog(id, 1024, NULL, infoLog);
			Logger::Error("shader failed to compile \"" + std::string(infoLog) + "\"");
		}
	}
	GLuint GetAttributeLocation(GLuint id, const std::string const& name)
	{
		//return location from glad
		UseShader(id);
		return glGetAttribLocation(id, name.c_str());
	}
	GLuint GetUniformLocation(GLuint id, const std::string const& name)
	{
		//return location from glad
		UseShader(id);
		return glGetUniformLocation(id, name.c_str());
	}
	GLuint CreateTexture(GLuint width, GLuint height)
	{
		//set basic settings and create texture of width and height
		GLuint id;
		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		return id;
	}
	GLuint CreateBuffer(GLuint index)
	{
		//create buffer bound to index and return id
		GLuint id;

		glGenBuffers(1, &id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);

		return id;
	}
	void DestroyBuffer(GLuint& id)
	{
		//delete buffer
		glDeleteBuffers(1, &id);
	}
	void DestroyShader(GLuint& id)
	{
		//delete program
		glDeleteProgram(id);
	}
	void DestroyTexture(GLuint& id)
	{
		//delete texture
		glDeleteTextures(1, &id);
	}
	void UseShader(GLuint id)
	{
		//use shader
		glUseProgram(id);
	}
	void SetShaderUInt(GLuint id, const std::string const& name, GLuint value)
	{
		//set value
		glUniform1ui(GetUniformLocation(id, name), value);
	}
	void SetShaderInt(GLuint id, const std::string const& name, GLint value)
	{
		//set value
		glUniform1i(GetUniformLocation(id, name), value);
	}
	void SetShaderIVec2(GLuint id, const std::string const& name, GLint x, GLint y)
	{
		//set value
		glUniform2i(GetUniformLocation(id, name), x, y);
	}
	void SetShaderFloat(GLuint id, const std::string const& name, GLfloat value)
	{
		//set value
		glUniform1f(GetUniformLocation(id, name), value);
	}
	void SetShaderMat4(GLuint id, const std::string const& name, const glm::mat4 const& value)
	{
		//set value
		glUniformMatrix4fv(GetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
	}
	void ReallocateBuffer(GLuint id, GLsizeiptr length, void* data)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, length, data, GL_DYNAMIC_DRAW);
	}
	void BufferData(GLuint id, GLintptr offset, GLsizeiptr length, void* data)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, length, data);
	}
}
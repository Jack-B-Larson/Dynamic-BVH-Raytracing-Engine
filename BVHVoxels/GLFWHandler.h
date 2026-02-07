#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
namespace GLFWHandler
{
	void Initialize();
	void Terminate();
	GLFWwindow* CreateWindow(int width, int height, std::string const& title, GLFWmonitor* monitor, GLFWwindow* share);
	void DestroyWindow(GLFWwindow* window);
	void UpdateWindowFullscreen(GLFWwindow* window, bool fullscreen);
	void CreateBuffers();
	void DestroyBuffers();
	void RenderElements(GLFWwindow* window);
	double GetTime();
	GLuint CreateShaderFromFile(const std::string const& path);
	void CompileShader(GLuint& id, const std::string const& source, GLenum type);
	GLuint GetAttributeLocation(GLuint id, const std::string const& name);
	GLuint GetUniformLocation(GLuint id, const std::string const& name);
	GLuint CreateTexture(GLuint width, GLuint height);
	GLuint CreateBuffer(GLuint index);
	void DestroyBuffer(GLuint& id);
	void DestroyShader(GLuint& id);
	void DestroyTexture(GLuint& id);
	void UseShader(GLuint id);
	void SetShaderUInt(GLuint id, const std::string const& name, GLuint value);
	void SetShaderInt(GLuint id, const std::string const& name, GLint value);
	void SetShaderIVec2(GLuint id, const std::string const& name, GLint x, GLint y);
	void SetShaderFloat(GLuint id, const std::string const& name, GLfloat value);
	void SetShaderMat4(GLuint id, const std::string const& name, const glm::mat4 const& value);

	void ReallocateBuffer(GLuint id, GLsizeiptr length, void* data);
	void BufferData(GLuint id, GLintptr offset, GLsizeiptr length, void* data);
	namespace
	{
		bool isInitialized = false;
		GLuint VAO = -1;
		GLuint VBO = -1;
		GLuint EBO = -1;
	}
}
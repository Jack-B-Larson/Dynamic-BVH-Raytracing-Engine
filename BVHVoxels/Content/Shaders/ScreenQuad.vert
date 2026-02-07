#version 460 core

in vec3 vertexPosition;
in vec2 vertexUV;

out vec2 textureCoordinates;

void main()
{
	textureCoordinates = vertexUV;
	gl_Position = vec4(vertexPosition, 1.0);
}
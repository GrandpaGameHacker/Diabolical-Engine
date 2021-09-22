#include "RenderingSystems.h"
#include "..\WindowManager.h"
#include <iostream>

bool BasicRendererSystem::Initialize(BasicRenderer* renderer)
{
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";

	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	auto wsize = WindowManager::GetWindowSize();
	GLuint vertexShader;
	GLuint fragmentShader;
	renderer->Width = wsize.width;
	renderer->Height = wsize.height;
	glViewport(0, 0, renderer->Width, renderer->Height);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	renderer->shaderProgram = glCreateProgram();
	glAttachShader(renderer->shaderProgram, vertexShader);
	glAttachShader(renderer->shaderProgram, fragmentShader);
	glLinkProgram(renderer->shaderProgram);
	glGetProgramiv(renderer->shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(renderer->shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f, // Left  
	 0.5f, -0.5f, 0.0f, // Right 
	 0.0f,  0.5f, 0.0f  // Top   
	};

	glGenVertexArrays(1, &renderer->VAO);
	glGenBuffers(1, &renderer->VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void BasicRendererSystem::Render(BasicRenderer& renderer)
{
	glClearColor(0.f, 0.f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderer.shaderProgram);
	glBindVertexArray(renderer.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(WindowManager::GetSDLWindow());
}

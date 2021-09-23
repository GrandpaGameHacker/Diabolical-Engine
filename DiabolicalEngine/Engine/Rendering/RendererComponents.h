#pragma once
#include "glm/glm.hpp"
#include "gl\glew.h"
#include <string>

struct BasicRenderer
{
	GLuint VBO, VAO;
	GLuint Width, Height;
	GLuint shaderProgram;
};
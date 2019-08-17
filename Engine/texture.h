#pragma once
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


typedef struct texture_t
{
	GLuint texID;
	int w;
	int h;
}texture_t;

texture_t* createTexture(const char* texturePath);


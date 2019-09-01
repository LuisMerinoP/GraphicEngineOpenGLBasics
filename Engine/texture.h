#pragma once
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#define TEXT_2D			0
#define TEXT_CUBEMAP	1

typedef struct texture_t
{
	GLuint texID;
	GLuint textType;
	int w;
	int h;
	std::vector<std::string>* textureName;
}texture_t;

void createCubeMapTexture(texture_t* newTex);
void createTexture(texture_t* texturePath);


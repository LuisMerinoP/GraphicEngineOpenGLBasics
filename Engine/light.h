#pragma once
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct light_t
{
	glm::vec3 color;
	float ambientalStrength;
	float diffuseStrength;
	glm::vec3 pos;
	int enabled;
}light_t;

light_t* createLight(glm::vec3 color, float ambientalStrength, float diffuseStrength, glm::vec3 pos);
void moveLight(light_t* light, glm::vec3 pos);
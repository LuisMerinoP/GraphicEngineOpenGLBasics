#pragma once
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct camera_t
{
	glm::vec3 pos;
	glm::vec3 lookAt;
	glm::vec3 up;

}camera_t;

void updateCamera(camera_t* cam, double* lastMX, double* lastMY, GLFWwindow* window);

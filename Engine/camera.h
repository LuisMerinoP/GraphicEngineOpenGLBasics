#pragma once
#include <stdio.h>
#include <stdlib.h>
/*visual no encuentra .dll glew. Con esto compilameos en estatico y
nuestro ejecutable será mas grande pero no importa*/
#define GLEW_STATIC 
//Aconsejable poner siempre directorios en mayúsculas
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct camera_t
{
	glm::vec3 position = glm::vec3(0, 0, 2);//default pos
	glm::vec3 lookAt;//point we are looking at
	glm::vec3 up;//our up direction.
}camera_t;

void updateCamera(camera_t* cam, double* lastMX, double* lastMY, GLFWwindow* window);
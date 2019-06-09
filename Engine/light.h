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

typedef struct light_t
{
	glm::vec3 color;
	float ambientalStrength;
	float diffuseStrength;
	glm::vec3 pos;
}light_t;

light_t* createLight(glm::vec3 color, float ambStrength, float diffuseStrength, glm::vec3 pos);
void moveLight(light_t* light, glm::vec3 pos);
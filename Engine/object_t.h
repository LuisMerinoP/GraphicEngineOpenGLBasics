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
#include "shaderSource.h"
#include "loaderMSH.h"
#include <list>
#define GLEW_STATIC 
//Aconsejable poner siempre directorios en mayúsculas
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderSource.h"
#include "loaderMSH.h"
#include <list>

#include "camera.h"
#include "light.h"

typedef struct object_t
{
	std::list<mesh_t*>* meshList;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
	glm::mat4 modelMtx;

}object_t;


object_t* createObject(const char* mshFile, GLuint programID);
void uploadMesh(mesh_t* pol, GLuint programID);
void updateObject(object_t* obj);
void drawObject(object_t* obj, camera_t cam, GLuint programID, light_t* light);
void moveObj(object_t* obj);

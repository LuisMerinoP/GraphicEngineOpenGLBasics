#pragma once
#include <stdio.h>
#include <stdlib.h>
/*visual no encuentra .dll glew. Con esto compilameos en estatico y
nuestro ejecutable será mas grande pero no importa*/
#define GLEW_STATIC 
//Aconsejable poner siempre directorios en mayúsculas
#include <GL/glew.h>
typedef struct polygon
{
	char* textureName;
	float* triangles;
	int* vertexIndex;
	int triangleCount;
	GLuint bufferVertexID; //vertex buffer
	GLuint bufferIndexID; //index buffer
	GLuint vertexArrayID; //vertex array (object)
}polygon;

polygon* loadMSH(const char* fileName);
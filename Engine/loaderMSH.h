#pragma once
#include <stdio.h>
#include <stdlib.h>
/*visual no encuentra .dll glew. Con esto compilameos en estatico y
nuestro ejecutable será mas grande pero no importa*/
#define GLEW_STATIC 
//Aconsejable poner siempre directorios en mayúsculas
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture.h"
#include <list>

//typedef struct vertex_t
//{
//	glm::vec3 pos;
//	glm::vec2 texCoord;
//	glm::vec3 normal;
//}vertex_t;
//
//typedef struct polygon
//{
//	char* textureName;
//	vertex_t* vertices;//buffer de datos por vertice<posicion xyz> <textura u,v> <normal xyz> 
//	int* vertexIndex;
//	int vertexIndexCount;
//	//en ppio estas variables se pueden inicializar desde código según cargamos la malla
//	int triangleCount;
//	int vertexCount;
//	int texCoordCount;
//	int normalCount;
//	int vertexCompCount;//numero de elementos/componentes de nuestro vértice. Pueden ser 3 o 4 según tengamos en cuenta o no la coordenada proyectiva. Si lo almacenamos lo podemos usar en el código
//	int texCoordCompCount;//numero de elementos/componentes de nuestra textura. La almacenamos por si puede ser variable para utilizarla en el código
//	int normalsCompCount;
//	int stride;//almacena la suma de lo que ocupan componentes de variables. Variable auxiliar para almacenar lo que ocupa la variable vertex. Stride quiere decir "paso"
//
//	GLuint bufferVertexID; //vertex buffer
//	GLuint bufferIndexID; //index buffer
//	GLuint vertexArrayID; //vertex array (object)
//}polygon;
//
//polygon* loadMSH(const char* fileName);


typedef struct vertex_t
{
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
}vertex_t;

typedef struct mesh_t
{
	char* textureName;
	vertex_t* vertices;//buffer datos por vertice<posicion x,y,z> <textura u,v> <normal <x,y,z>
	int* vertexIndex;
	int vertexIndexCount;
	int triangleCount;
	int vertexCount;
	int texCoordCount;
	int normalsCount;
	int vertexCompCount;
	int texCoordCompCount;
	int normalsCompCount;
	int stride;

	GLuint bufferID; //vertex buffer
	GLuint bufferIndexID; // index buffer
	GLuint vertexArrayID; //vertex array (objeto)

	texture_t* texture;

}mesh_t;

std::list<mesh_t*>* loadMSH(const char* fileName);
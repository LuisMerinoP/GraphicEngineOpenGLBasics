#include "billboard.h"

object_t* createBillboard(const char* texture, GLuint programID) {
	mesh_t* bboardMesh = new mesh_t;
	bboardMesh->vertexCompCount = 3;
	bboardMesh->normalsCompCount = 3;
	bboardMesh->texCoordCompCount = 2;

	bboardMesh->normalsCount = 6;
	bboardMesh->vertexCount = 6;
	bboardMesh->texCoordCount = 6;

	bboardMesh->textureName = new char[strlen(texture) + 1];
	memcpy(bboardMesh->textureName, texture, strlen(texture) + 1);

	bboardMesh->vertices = new vertex_t[6];//buffer datos por vertice<posicion x,y,z> <textura u,v> <normal <x,y,z>
	bboardMesh->vertexIndex = new int[6];
	bboardMesh->vertexIndexCount = 6;
	bboardMesh->triangleCount = 2;
	bboardMesh->stride = 8;

	for (int i = 0; i < 6; i++)
	{
		bboardMesh->vertexIndex[i] = i;
		bboardMesh->vertices[i].normal = glm::vec3(0, 0, 1);
	}

	bboardMesh->vertices[0].texCoord = glm::vec2(0.0, 0.0);
	bboardMesh->vertices[0].pos = glm::vec3(-0.5, -0.5, 0);
	bboardMesh->vertices[1].texCoord = glm::vec2(1.0, 0.0);
	bboardMesh->vertices[1].pos = glm::vec3(0.5, -0.5, 0);
	bboardMesh->vertices[2].texCoord = glm::vec2(1.0, 1.0);
	bboardMesh->vertices[2].pos = glm::vec3(0.5, 0.5, 0);

	bboardMesh->vertices[3].texCoord = glm::vec2(0.0, 0.0);
	bboardMesh->vertices[3].pos = glm::vec3(-0.5, -0.5, 0);
	bboardMesh->vertices[4].texCoord = glm::vec2(1.0, 1.0);
	bboardMesh->vertices[4].pos = glm::vec3(0.5, 0.5, 0);
	bboardMesh->vertices[5].texCoord = glm::vec2(0.0, 1.0);
	bboardMesh->vertices[5].pos = glm::vec3(-0.5, 0.5, 0);


	object_t * newObj = new object_t;
	newObj->meshList = new std::list<mesh_t*>();
	newObj->meshList->push_back(bboardMesh);

	newObj->modelMtx = glm::mat4(1.0);
	newObj->position = glm::vec3(0, 0, 0);
	newObj->rotation = glm::vec3(0, 0, 0);
	newObj->scaling = glm::vec3(1, 1, 1);

	for (std::list<mesh_t*>::iterator it = newObj->meshList->begin();
		it != newObj->meshList->end();
		it++
		)
	{
		(*it)->texture = createTexture((*it)->textureName);
		uploadMesh((*it), programID);
	}

	return newObj;
}


void updateBillboard(object_t* bboard, camera_t* cam)
{
	glm::mat4  visor = glm::lookAt(cam->pos, cam->lookAt, cam->up);
	bboard->modelMtx = glm::transpose(visor);
	bboard->modelMtx[0][3] = 0;
	bboard->modelMtx[1][3] = 0;
	bboard->modelMtx[2][3] = 0;
	bboard->modelMtx[3] = glm::vec4(bboard->position, 1);
}

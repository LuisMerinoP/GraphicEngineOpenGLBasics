#include "object_t.h"



void uploadMesh(mesh_t* pol, GLuint programID){
	glUseProgram(programID);
	GLint vpos = glGetAttribLocation(programID, "vpos");
	GLint vtex = glGetAttribLocation(programID, "vtex");
	GLint vnormal = glGetAttribLocation(programID, "vnormal");
	GLint vtangent = glGetAttribLocation(programID, "vtangent");

	glGenVertexArrays(1, &(pol->vertexArrayID));
	glGenBuffers(1, &(pol->bufferID));
	glGenBuffers(1, &(pol->bufferIndexID));

	glBindVertexArray(pol->vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, pol->bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pol->stride*pol->vertexCount, pol->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pol->bufferIndexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * pol->vertexIndexCount, pol->vertexIndex, GL_STATIC_DRAW);

	glVertexAttribPointer(vpos, pol->vertexCompCount, GL_FLOAT, GL_FALSE, pol->stride * sizeof(float), nullptr);
	glEnableVertexAttribArray(vpos);

	glVertexAttribPointer(vtex, pol->texCoordCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), (void*)(pol->vertexCompCount * sizeof(float)));
	glEnableVertexAttribArray(vtex);

	glVertexAttribPointer(vnormal, pol->normalsCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), (void*)((pol->vertexCompCount + pol->texCoordCompCount) * sizeof(float)));
	glEnableVertexAttribArray(vnormal);

	glVertexAttribPointer(vtangent, pol->tangentsCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), (void*)((pol->vertexCompCount + pol->texCoordCompCount + pol->normalsCompCount) * sizeof(float)));
	glEnableVertexAttribArray(vtangent);
}

void drawMesh(glm::mat4 model, mesh_t* pol, GLuint programID, camera_t cam, light_t* light){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(pol->vertexArrayID);

	glUseProgram(programID);

	glm::mat4  visor = glm::lookAt(cam.pos, cam.lookAt, cam.up);
	glm::mat4  proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 1000.0f);
	glm::mat4 MVP = proyection * visor * model;
	glm::mat4 normalMat = glm::transpose(glm::inverse(visor*model));

	GLuint model_ID = glGetUniformLocation(programID, "model");
	GLuint view_ID = glGetUniformLocation(programID, "view");
	GLuint projection_ID = glGetUniformLocation(programID, "projection");
	GLuint normalMat_ID = glGetUniformLocation(programID, "normalMat");

	GLuint texSampler_ID = glGetUniformLocation(programID, "samplerColor");
	GLuint texSamplerNormal_ID = glGetUniformLocation(programID, "samplerNormal");
	GLuint texSamplerCubeMap_ID = glGetUniformLocation(programID, "samplerCubeMap");

	GLuint lightEnable_ID = glGetUniformLocation(programID, "lightEnable");
	GLuint useNormalTex_ID = glGetUniformLocation(programID, "useNormalTex");
	GLuint useSamplerCube_ID = glGetUniformLocation(programID, "useSamplerCube");
	GLuint shininess_ID = glGetUniformLocation(programID, "shininess");


	glm::vec4 color = glm::vec4(1, 1, 0, 0);
	glUniformMatrix4fv(model_ID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(view_ID, 1, GL_FALSE, &visor[0][0]);
	glUniformMatrix4fv(projection_ID, 1, GL_FALSE, &proyection[0][0]);
	glUniformMatrix4fv(normalMat_ID, 1, GL_FALSE, &normalMat[0][0]);


	glUniform1i(lightEnable_ID, light->enabled);

	if (pol->texture->textType == TEXT_2D)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pol->texture->texID);
		glUniform1i(texSampler_ID, 0);
		glUniform1i(useSamplerCube_ID, 0);
	}

	if (pol->useNormalTex)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pol->normalTexture->texID);
		glUniform1i(texSamplerNormal_ID, 1);
		glUniform1i(useSamplerCube_ID, 0);
		glUniform1i(useNormalTex_ID, 1);
	}

	if (pol->texture->textType == TEXT_CUBEMAP)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pol->texture->texID);
		glUniform1i(texSamplerCubeMap_ID, 2);
		glUniform1i(useSamplerCube_ID, 1);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glUniform1i(lightEnable_ID, 0);
	}

	glUniform1i(shininess_ID, 16);

	GLuint lightColor_ID = glGetUniformLocation(programID, "lightColor");
	GLuint lightPos_ID = glGetUniformLocation(programID, "lightPos");
	GLuint eyePos_ID = glGetUniformLocation(programID, "eyePos");
	GLuint ambientStrenght_ID = glGetUniformLocation(programID, "ambientStrenght");

	glUniform3f(lightColor_ID, light->color.x, light->color.y, light->color.z);
	glUniform1f(ambientStrenght_ID, light->ambientalStrength);
	glUniform3f(lightPos_ID, light->pos.x, light->pos.y, light->pos.z);
	glUniform3f(eyePos_ID, cam.pos.x, cam.pos.y, cam.pos.z);

	if (pol->useDepthWrite)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glDrawElements(GL_TRIANGLES, pol->vertexIndexCount, GL_UNSIGNED_INT, nullptr);
}



object_t* createObject(const char* mshFile, GLuint programID){
	object_t* newObj = new object_t;
	newObj->modelMtx = glm::mat4(1.0);
	newObj->position = glm::vec3(0, 0, 0);
	newObj->rotation = glm::vec3(0, 0, 0);
	newObj->scaling = glm::vec3(1, 1, 1);
	newObj->meshList = loadMSH(mshFile);

	for (std::list<mesh_t*>::iterator it = newObj->meshList->begin();
		it != newObj->meshList->end();
		it++
		)
	{
		if ((*it)->texture->textureName->size() == 1)
			createTexture((*it)->texture);
		else if ((*it)->texture->textureName->size() == 6)
			createCubeMapTexture((*it)->texture);

		if ((*it)->useNormalTex)
			createTexture((*it)->normalTexture);

		uploadMesh((*it), programID);
	}
	return newObj;
}

void updateObject(object_t* obj)
{

	glm::mat4  trans = glm::translate(glm::mat4(1.0), obj->position);
	glm::mat4  rot1 = glm::rotate(glm::mat4(1.0), obj->rotation.x, glm::vec3(1, 0, 0));
	glm::mat4  rot2 = glm::rotate(glm::mat4(1.0), obj->rotation.y, glm::vec3(0, 1, 0));
	glm::mat4  rot3 = glm::rotate(glm::mat4(1.0), obj->rotation.z, glm::vec3(0, 0, 1));

	glm::mat4  scaling = glm::scale(glm::mat4(1.0), obj->scaling);

	obj->modelMtx = scaling * rot1*rot2*rot3*trans;

}


void drawObject(object_t* obj, camera_t cam, GLuint programID, light_t* light)
{
	for (std::list<mesh_t*>::iterator it = obj->meshList->begin();
		it != obj->meshList->end();
		it++
		)
	{
		drawMesh(obj->modelMtx, (*it), programID, cam, light);
	}
}

void moveObj(object_t* obj)
{

}
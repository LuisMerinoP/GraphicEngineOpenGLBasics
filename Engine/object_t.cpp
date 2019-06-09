#include "object_t.h"


void uploadMeshGPU(mesh_t* pol, GLuint programID)
{
	glUseProgram(programID);
	//posición de variables vpos y vtex
	GLuint vpos = glGetAttribLocation(programID, "vpos");
	GLuint vtex = glGetAttribLocation(programID, "vtex");
	GLuint vnormal = glGetAttribLocation(programID, "vnormal");

	glGenVertexArrays(1, &(pol->vertexArrayID));
	glGenBuffers(1, &(pol->bufferVertexID));
	glGenBuffers(1, &(pol->bufferIndexID));

	glBindVertexArray(pol->vertexArrayID);//vincular array de vertices

	//primero subo vértices
	glBindBuffer(GL_ARRAY_BUFFER, pol->bufferVertexID); //vincular vertexID
	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los vértices del pol (pol->triangles)
	//glBufferData(GL_ARRAY_BUFFER,sizeof(float)*18,pol->triangles, GL_STATIC_DRAW); 
	//Cambiamos la línea anterior para que los datos estén en función de los datos de nuestro pol
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pol->stride*pol->vertexCount, pol->vertices, GL_STATIC_DRAW);

	//segundo subo índices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pol->bufferIndexID); //vincular bufferID
	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los vértices del pol (pol->triangles)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * pol->vertexIndexCount, pol->vertexIndex, GL_STATIC_DRAW);//subir array de índices

	/*Hay que decirle a opengl en que formato estan los datos.
	Para que opengl entienda el formato en que subimos los datos a GPU hay que indicarselo aqui
	con esta función.*/
	glVertexAttribPointer(vpos, pol->vertexCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), nullptr);
	glEnableVertexAttribArray(vpos);

	glVertexAttribPointer(vtex, pol->texCoordCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), (void*)(pol->vertexCompCount * sizeof(float)));
	glEnableVertexAttribArray(vtex);
	glVertexAttribPointer(vnormal, pol->normalsCompCount, GL_FLOAT, GL_FALSE,
		pol->stride * sizeof(float), (void*)((pol->vertexCompCount + pol->texCoordCompCount) * sizeof(float)));
	glEnableVertexAttribArray(vnormal);
}


void drawMesh(glm::mat4 model, mesh_t* pol, GLuint programID, camera_t cam, light_t* light)
{
	//Para dibujar desde GPU:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/*no debería ser necesario por que no hemos seleccionado ningún otro objeto pero añadimos por si acaso.
	En caso de tener varios elementos habría que vincular el correspondiente al que queremos dibujar!*/
	glBindVertexArray(pol->vertexArrayID);
	glUseProgram(programID);

	//z positiva hacia atras. Para retrasar z ponemos 2.
	glm::mat4 view = glm::lookAt(cam.position, cam.lookAt, cam.up);
	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//not needed since with ligth implementation MVP multiplication is done in the shader
	glm::mat4 MVP = proyection * view * model;

	//pasarle matriz de traslación al shader
	//GLuint MVP_ID = glGetUniformLocation(programID, "MVP"); //with light implemetation MVP is separated in model, view projection in the shader
	GLuint model_ID = glGetUniformLocation(programID, "model");
	GLuint view_ID = glGetUniformLocation(programID, "view");
	GLuint projection_ID = glGetUniformLocation(programID, "projection");

	//GLuint color_ID = glGetUniformLocation(programID, "vcolor");
	//glm::vec4 color = glm::vec4(1, 1, 0, 0);

	GLuint texSamplerID = glGetUniformLocation(programID, "sampler");

	GLuint lightEnabled_ID = glGetUniformLocation(programID, "lightEnabled");

	//with light implemetation MVP is separated in model, view projection in the shader
	//glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posición inicial del array.//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
	glUniformMatrix4fv(model_ID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(view_ID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projection_ID, 1, GL_FALSE, &proyection[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pol->texture->texID);//Para solo una textura no haría falta esta línea. Para mas, sí. Hay que bindear cada vez.
	glUniform1i(texSamplerID, 0);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);//introducimos variables del pol
	
	//light not enabled
	glUniform1i(lightEnabled_ID, 0);


	//get the light variables IDs of the shader 
	GLuint lightColor_ID = glGetUniformLocation(programID, "lightColor");
	GLuint ambientStrenght_ID = glGetUniformLocation(programID, "ambientStrength");
	GLuint lightPos_ID = glGetUniformLocation(programID, "lightPos");
	GLuint eyePos_ID = glGetUniformLocation(programID, "eyePos");


	//upload uniforms
	glUniform3f(lightColor_ID, light->color.x, light->color.y, light->color.z);
	glUniform1f(ambientStrenght_ID, light->ambientalStrength);
	glUniform3f(lightPos_ID, light->pos.x, light->pos.y, light->pos.z);
	glUniform3f(eyePos_ID, cam.position.x, cam.position.y, cam.position.z);


	glDrawElements(GL_TRIANGLES, pol->vertexIndexCount, GL_UNSIGNED_INT, nullptr);

}

object_t* createObject(const char* mshFile, GLuint programID)
{
	object_t* newObj = new object_t;
	newObj->modelMatrix = glm::mat4(1.0);
	newObj->position = glm::vec3(0,0,0);
	newObj->rotation = glm::vec3(0,0,0);
	newObj->scale = glm::vec3(1,1,1);

	newObj->meshList = loadMSH(mshFile);
	//for each mesh we need to load corresponding texture
	for (std::list<mesh_t*>::iterator it = newObj->meshList->begin();
		it != newObj->meshList->end();
		it++)
	{
		(*it)->texture = createTexture((*it)->textureName);
		uploadMeshGPU((*it), programID);
	}
	return newObj;
}

void updateObject(object_t* obj)
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0), obj->position);
	glm::mat4  rot1 = glm::rotate(glm::mat4(1.0), obj->rotation.x, glm::vec3(1, 0, 0));
	glm::mat4  rot2 = glm::rotate(glm::mat4(1.0), obj->rotation.y, glm::vec3(0, 1, 0));
	glm::mat4  rot3 = glm::rotate(glm::mat4(1.0), obj->rotation.z, glm::vec3(0, 0, 1));

	obj->modelMatrix = rot1*rot2*rot3*trans;
}
void drawObject(object_t* obj, camera_t cam, GLuint programID, light_t* light)
{
	for (std::list<mesh_t*>::iterator it = obj->meshList->begin();
		it != obj->meshList->end();
		it++)
	{
		drawMesh(obj->modelMatrix, (*it), programID, cam, light);
	}
}
void moveObj(object_t* obj)
{

}
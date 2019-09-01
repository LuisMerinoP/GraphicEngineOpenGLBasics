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
#define STB_IMAGE_IMPLEMENTATION
#include "loaderMSH.h"

#include "camera.h"
#include "object.h"
#include "light.h"
#include "billboard.h"

#include "emitter.h"

float pos[4] = { 0.0,0.0,0.0,1.0};
//float pos1[4] = { 3.0,0.0,0.0,1.0 };
float rotAngle = 0;

float triangleInit[18] = {
	0.0,0.5,0.0,1.0, 0.5,0.99,
	-0.5,0.0,0.0,1.0, 0.0,0.0,
	0.5,0.0,0.0,1.0, 1.0,0.0
};
float triangle[12] = {
	0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0, 
	0.0,0.0,0.0,0.0
};

//void uploadPolygonGPU(polygon* pol, GLuint programID)
//{
//	glUseProgram(programID);
//	//posición de variables vpos y vtex
//	GLuint vpos = glGetAttribLocation(programID, "vpos");
//	GLuint vtex = glGetAttribLocation(programID, "vtex");
//	GLuint vnormal = glGetAttribLocation(programID, "vnormal");
//
//	glGenVertexArrays(1, &(pol->vertexArrayID));
//	glGenBuffers(1, &(pol->bufferVertexID));
//	glGenBuffers(1, &(pol->bufferIndexID));
//
//	glBindVertexArray(pol->vertexArrayID);//vincular array de vertices
//	
//	//primero subo vértices
//	glBindBuffer(GL_ARRAY_BUFFER, pol->bufferVertexID); //vincular vertexID
//	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los vértices del pol (pol->triangles)
//	//glBufferData(GL_ARRAY_BUFFER,sizeof(float)*18,pol->triangles, GL_STATIC_DRAW); 
//	//Cambiamos la línea anterior para que los datos estén en función de los datos de nuestro pol
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pol->stride*pol->vertexCount, pol->vertices, GL_STATIC_DRAW);
//
//	//segundo subo índices
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pol->bufferIndexID); //vincular bufferID
//	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los vértices del pol (pol->triangles)
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * pol->vertexIndexCount,pol->vertexIndex, GL_STATIC_DRAW);//subir array de índices
//
//	/*Hay que decirle a opengl en que formato estan los datos. 
//	Para que opengl entienda el formato en que subimos los datos a GPU hay que indicarselo aqui
//	con esta función.*/
//	glVertexAttribPointer(vpos,pol->vertexCompCount,GL_FLOAT,GL_FALSE,
//		pol->stride*sizeof(float),nullptr);
//	glEnableVertexAttribArray(vpos);
//
//	glVertexAttribPointer(vtex,pol->texCoordCompCount,GL_FLOAT,GL_FALSE, 
//		pol->stride*sizeof(float),(void*)(pol->vertexCompCount*sizeof(float)));
//	glEnableVertexAttribArray(vtex);
//	glVertexAttribPointer(vnormal, pol->normalsCompCount, GL_FLOAT, GL_FALSE, 
//		pol->stride * sizeof(float), (void*)((pol->vertexCompCount + pol->texCoordCompCount) * sizeof(float)));
//	glEnableVertexAttribArray(vnormal);
//}

void MultMatrix(float* mtx,int row, int col, float* v1, float* vres)
{
	for(int i=0;i<row;i++)
	{ 
		vres[i] = 0;
		for (int j=0;j<col;j++)
		{
			vres[i] += mtx[i*col + j] * v1[j];
		}
	}
}

void Move(float move_x, float move_y, float move_z)
{
	float MTranslation[] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	MTranslation[3] = move_x;
	MTranslation[7] = move_y;
	MTranslation[11] = move_z;

	for (int i = 0; i < 3; i++)
		MultMatrix(MTranslation, 4, 4, &triangleInit[i*4], &triangle[i*4]);
}

////Implementamos objeto CAMERA
//void DrawTriangle(polygon* pol, GLuint programID, camera_t cam, GLuint texID, light_t* light)
//{
//	//Para dibujar desde GPU:
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	/*no debería ser necesario por que no hemos seleccionado ningún otro objeto pero añadimos por si acaso.
//	En caso de tener varios elementos habría que vincular el correspondiente al que queremos dibujar!*/
//	glBindVertexArray(pol->vertexArrayID);
//
//	glUseProgram(programID);
//
//	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos[0], pos[1], pos[2]));
//	glm::mat4  rot = glm::rotate(glm::mat4(1.0), rotAngle, glm::vec3(0, 1, 0));
//	trans = trans * rot;
//	//z positiva hacia atras. Para retrasar z ponemos 2.
//	glm::mat4 view = glm::lookAt(cam.position,cam.lookAt, cam.up);
//
//	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//
//	//not needed since with ligth implementation MVP multiplication is done in the shader
//	//glm::mat4 MVP = proyection * view * trans;
//	
//	//pasarle matriz de traslación al shader
//	//GLuint MVP_ID = glGetUniformLocation(programID, "MVP"); //with light implemetation MVP is separated in model, view projection in the shader
//	GLuint model_ID = glGetUniformLocation(programID, "model");
//	GLuint view_ID = glGetUniformLocation(programID, "view");
//	GLuint projection_ID = glGetUniformLocation(programID, "projection");
//
//	//GLuint color_ID = glGetUniformLocation(programID, "vcolor");
//	//glm::vec4 color = glm::vec4(1, 1, 0, 0);
//
//	GLuint texSamplerID = glGetUniformLocation(programID, "sampler");
//
//	//with light implemetation MVP is separated in model, view projection in the shader
//	//glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posición inicial del array.//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
//	glUniformMatrix4fv(model_ID, 1, GL_FALSE, &trans[0][0]);
//	glUniformMatrix4fv(view_ID, 1, GL_FALSE, &view[0][0]);
//	glUniformMatrix4fv(projection_ID, 1, GL_FALSE, &proyection[0][0]);
//	
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texID);//Para solo una textura no haría falta esta línea. Para mas, sí. Hay que bindear cada vez.
//	glUniform1i(texSamplerID,0);
//	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);//introducimos variables del pol
//	
//	
//	//get the light variables IDs of the shader 
//	GLuint lightColor_ID = glGetUniformLocation(programID, "lightColor");
//	GLuint ambientStrenght_ID = glGetUniformLocation(programID, "ambientStrength");
//	GLuint lightPos_ID = glGetUniformLocation(programID, "lightPos");
//	GLuint eyePos_ID = glGetUniformLocation(programID, "eyePos");
//
//
//	//upload uniforms
//	glUniform3f(lightColor_ID, light->color.x, light->color.y, light->color.z);
//	glUniform1f(ambientStrenght_ID, light->ambientalStrength);
//	glUniform3f(lightPos_ID, light->pos.x, light->pos.y, light->pos.z);
//	glUniform3f(eyePos_ID, cam.position.x, cam.position.y, cam.position.z);
//
//
//	glDrawElements(GL_TRIANGLES, pol->vertexIndexCount, GL_UNSIGNED_INT, nullptr);
//
//}

void mouseManager(GLFWwindow* win, double xpos, double ypos)
{
	pos[0] = (xpos / 640) - 0.5;
	pos[1] = (-ypos / 480) + 0.2;
}
camera_t cam;

void keyBoardManager(GLFWwindow* win, int key, int scancode, int action, int mods)
{

	switch (key)
	{
		case GLFW_KEY_A:
		{
			//pos[0] -= 0.1;
			cam.pos.x -= 0.1;
			cam.lookAt.x -= 0.1;
		}
		break;
		case GLFW_KEY_D:
		{
			//pos[0] += 0.1;
			cam.pos.x += 0.1;
			cam.lookAt.x += 0.1;
		}
		break;
		case GLFW_KEY_W:
		{
			//pos[1] += 0.1;
			cam.pos.z -= 0.1;
			cam.lookAt.z -= 0.1;
		}
		break;
		case GLFW_KEY_S:
		{
			//pos[1] -= 0.1;
			cam.pos.z += 0.1;
			cam.lookAt.z += 0.1;
		}
		break;
		case GLFW_KEY_G:
		{
			//pos[1] += 0.1;
			cam.pos.y -= 0.1;
			cam.lookAt.y -= 0.1;
		}
		break;
		case GLFW_KEY_T:
		{
			//pos[1] -= 0.1;
			cam.pos.y += 0.1;
			cam.lookAt.y += 0.1;
		}
		break;
		case GLFW_KEY_F:
		{
			rotAngle += 0.5;
		}
		break;
		default:
			break;
		}
}


/*argc y argv son argumentos para cuando hacemos aplicaciones de terminal.
Por si se ejecuta la app con nombre de la app + espacio + argumentos extra.
P.ej si le queires pasar alguna imagen inicial o algún parámetro de entrada
argc es el num de argumentos y argv puntero a char de carácteres en el
que se describiría cada argumento.
argc vale 1 y argv va a tener en primera posición el nombre del programa
y despues el resto de paramentros que quieras pasarle*/

//int main(int argc, char** argv)
//{
//	glfwInit();
//	//crear ventana
//	GLFWwindow* win1 = glfwCreateWindow(640, 480,"Hola mundo glfw", nullptr,nullptr);
//	//ponerla delante
//	glfwMakeContextCurrent(win1);
//	//no redimensionable
//	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//	//callback de teclado. Ventana y función como argumentos
//	glfwSetKeyCallback(win1,KeyBoardManager);
//	//callback de ratón
//	glfwSetCursorPosCallback(win1,MouseManager);
//
//	glewInit();//hay que ponerlo despues de glfwInit();
//
//	glEnable(GL_DEPTH_TEST);
//
//	cam.lookAt = glm::vec3(0, 0, 0);
//	cam.position = glm::vec3(0, 0, 2);//recordar que hacia atras es positivo
//	cam.up = glm::vec3(0, 1, 0);
//	double lastX, lastY;
//	lastX = lastY = 0;
//
//	//polygon* pol = createPolygon(); //Antes de implementar carga de xml. Una vez implementada:
//	//polygon* pol = loadMSH("triangulo.msh");
//	polygon* pol = loadMSH("cubo.msh");
//	GLint programID = compileAndLinkShaderProgram(vertexShaderSRC, fragmentShaderSRC);
//	uploadPolygonGPU(pol,programID);
//
//	//GLuint texID = cargaTextura("data/top.png");
//	GLuint texID = cargaTextura(pol->textureName);
//
//
//	//crate light. white color = (1,1,1)
//	light_t* light = createLight(glm::vec3(1, 1, 1), 0.5, 0.5, glm::vec3(0, 0, 2));
//
//	//mientras no cerrada {}. Tenemos que hacer glfwSwapBuffers(win1) al final para que se pinte en pantalla
//	while (!(glfwWindowShouldClose(win1)))
//	{
//		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //memory buffer reset
//		updateCamera(&cam, &lastX, &lastY, win1);
//		DrawTriangle(pol, programID, cam, texID, light);
//		//DrawTriangle2(pol, programID, cam, texID);
//		glfwSwapBuffers(win1);
//		glfwPollEvents();
//	}
//	glfwTerminate();
//	return 0;
//}


int main(int argc, char** argv)
{
	glfwInit();
	//crear ventana
	GLFWwindow* win1 = glfwCreateWindow(640, 480, "Holamundo glfw", nullptr, nullptr);
	glfwMakeContextCurrent(win1);
	//atributos resize
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetKeyCallback(win1, keyBoardManager);
	glfwSetCursorPosCallback(win1, mouseManager);
	//glfwSetInputMode(win1, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//mientras (no cerrada)
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	cam.lookAt = glm::vec3(0, 0, 0);
	cam.pos = glm::vec3(0, 0, 2);
	cam.up = glm::vec3(0, 1, 0);
	double lastX, lastY;
	lastX = lastY = 0;


	GLint programID = compileAndLinkShaderProgram(vertexShaderSRC, fragmentShaderSRC);

	object_t* obj = createObject("data/normalMapCube.msh", programID);//createPolygon();
	//object_t* obj = createBillboard("data/smoke.png", programID);
	
	/*emitter_t* emi1 = createEmitter("data/smoke.png", programID, 2, 5, 0.1, 0.3, glm::vec3(-0.2, 0.5, -0.5), glm::vec3(0.2, 5.5, 0.5),
		glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 2);

	
	object_t* obj2 = createBillboard("data/top.png", programID);
	obj2->position = glm::vec3(0.0f, -1.0f, 0.0f);*/

	light_t* light = createLight(glm::vec3(1, 1, 1), 0.5, 0.5, glm::vec3(0, 0, 2));
	light->enabled = 1;
	double initTime = 0;

	while (!(glfwWindowShouldClose(win1)))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		updateCamera(&cam, &lastX, &lastY, win1);

		//updateEmitter(emi1, programID);
		//updateBillboard(obj, &cam);
		//updateBillboard(obj2, &cam);

		//drawEmitter(emi1, &cam, programID, light);
		//drawObject(obj, cam, programID, light);
		updateObject(obj);
		drawObject(obj, cam, programID, light);

		glfwSwapBuffers(win1);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


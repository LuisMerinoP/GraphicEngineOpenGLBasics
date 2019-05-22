#include <stdio.h>
#include <stdlib.h>
/*visual no encuentra .dll glew. Con esto compilameos en estatico y
nuestro ejecutable ser� mas grande pero no importa*/
#define GLEW_STATIC 
//Aconsejable poner siempre directorios en may�sculas
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderSource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "loaderMSH.h"

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

typedef struct camera_t
{
	glm::vec3 position;//position
	glm::vec3 lookAt;//point we are looking at
	glm::vec3 up;//our up direction.
}camera_t;

//polygon* createPolygon()
//{
//	//reservamos la estructura
//	polygon* pol = new polygon;
//	pol->triangleCount = 1;
//	pol->triangles = new float[18];//3 floats por v�rtice x 3 vertices = 9 floats
//	pol->vertexIndex = new int[3];//3 vertices, 3 posiciones.
//
//	//copiar triangle del main al pol�gono
//	for (int i = 0; i < 18; i++)
//	{
//		pol->triangles[i] = triangleInit[i];
//	}
//
//	//inicializo v�rtices
//	for (int i = 0; i < 3; i++)
//	{
//		pol->vertexIndex[i] = i;
//	}
//	return pol;
//}

void uploadPolygonGPU(polygon* pol, GLuint programID)
{
	glUseProgram(programID);
	//posici�n de variables vpos y vtex
	GLuint vpos = glGetAttribLocation(programID, "vpos");
	GLuint vtex = glGetAttribLocation(programID, "vtex");
	GLuint vnormal = glGetAttribLocation(programID, "vnormal");

	glGenVertexArrays(1, &(pol->vertexArrayID));
	glGenBuffers(1, &(pol->bufferVertexID));
	glGenBuffers(1, &(pol->bufferIndexID));

	glBindVertexArray(pol->vertexArrayID);//vincular array de vertices
	
	//primero subo v�rtices
	glBindBuffer(GL_ARRAY_BUFFER, pol->bufferVertexID); //vincular vertexID
	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los v�rtices del pol (pol->triangles)
	//glBufferData(GL_ARRAY_BUFFER,sizeof(float)*18,pol->triangles, GL_STATIC_DRAW); 
	//Cambiamos la l�nea anterior para que los datos est�n en funci�n de los datos de nuestro pol
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pol->stride*pol->vertexCount, pol->vertices, GL_STATIC_DRAW);

	//segundo subo �ndices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pol->bufferIndexID); //vincular bufferID
	//modificar buffer de memoria cargando datos. Los datos son las coordenadas de los v�rtices del pol (pol->triangles)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * pol->vertexIndexCount,pol->vertexIndex, GL_STATIC_DRAW);//subir array de �ndices

	/*Hay que decirle a opengl en que formato estan los datos. 
	Para que opengl entienda el formato en que subimos los datos a GPU hay que indicarselo aqui
	con esta funci�n.*/
	glVertexAttribPointer(vpos,pol->vertexCompCount,GL_FLOAT,GL_FALSE,
		pol->stride*sizeof(float),nullptr);
	glEnableVertexAttribArray(vpos);

	glVertexAttribPointer(vtex,pol->texCoordCompCount,GL_FLOAT,GL_FALSE, 
		pol->stride*sizeof(float),(void*)(pol->vertexCompCount*sizeof(float)));
	glEnableVertexAttribArray(vtex);
	glVertexAttribPointer(vnormal, pol->normalsCompCount, GL_FLOAT, GL_FALSE, 
		pol->stride * sizeof(float), (void*)((pol->vertexCompCount + pol->texCoordCompCount) * sizeof(float)));
	glEnableVertexAttribArray(vnormal);
}

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

//Antes de implementar objeto CAMERA
//void DrawTriangle(polygon* pol, GLuint programID)
//{
//	glClear(GL_COLOR_BUFFER_BIT); //memory buffer reset
//	
//
//	//Para dibujar desde GPU:
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	/*no deber�a ser necesario por que no hemos seleccionado ning�n otro objeto pero a�adimos por si acaso.
//	En caso de tener varios elementos habr�a que vincular el correspondiente al que queremos dibujar!*/
//	glBindVertexArray(pol->vertexArrayID); 
//
//	glUseProgram(programID);
//
//	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos[0], pos[1], pos[2]));
//	glm::mat4  rot = glm::rotate(glm::mat4(1.0), rotAngle, glm::vec3(0, 1, 0));
//	trans = trans * rot;
//	//z positiva hacia atras. Para retrasar z ponemos 2.
//	glm::mat4 visor = glm::lookAt(glm::vec3(0,0,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
//
//	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//
//	glm::mat4 MVP = proyection * visor*trans;
//	//pasarle matriz de traslaci�n al shader
//	GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
//
//	GLuint color_ID = glGetUniformLocation(programID, "vcolor");
//	//glm::vec4 color = glm::vec4(1, 1, 0, 0);
//
//	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posici�n inicial del array.
//	//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
//
//	glUniform4f(color_ID, 1.0f, 1.0f, 0, 0);
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
//
//}

GLuint cargaTextura(const char* tex)
{
	GLuint texID = -1;//para que si sale esto de error4
	glGenTextures(1, &texID);
	int x, y;
	x = y = 0;
	//llamada a stbi_load
	unsigned char* texBytes = stbi_load(tex, &x, &y, nullptr, 4);//cargados los bytes de la imagen
	//generar la textura en si
	
	//bindear (hacerla activa)
	/*Esta l�nea es cr�tica si queremos hacer mas cosas. En nuestro c�digo 
	no utilizaremos muchas texturas y nos valdr� con un texID y un bindeo.
	Pero cada vez que queramos utilizar una textura cargada hay que repetir el bind.
	Hay que poner el glBindTexture(GL_TEXTURE_2D, texID); justo antes de hacer cosas*/
	glBindTexture(GL_TEXTURE_2D, texID);
	//par�metros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBytes);
	//generar mipmaps (opcional)
	glGenerateMipmap(GL_TEXTURE_2D);
	//una vez cargada la imagen la puedo eliminar
	stbi_image_free(texBytes);

	return texID;
}



//Implementamos objeto CAMERA
void DrawTriangle(polygon* pol, GLuint programID, camera_t cam, GLuint texID)
{
	//Para dibujar desde GPU:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/*no deber�a ser necesario por que no hemos seleccionado ning�n otro objeto pero a�adimos por si acaso.
	En caso de tener varios elementos habr�a que vincular el correspondiente al que queremos dibujar!*/
	glBindVertexArray(pol->vertexArrayID);

	glUseProgram(programID);

	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos[0], pos[1], pos[2]));
	glm::mat4  rot = glm::rotate(glm::mat4(1.0), rotAngle, glm::vec3(0, 1, 0));
	trans = trans * rot;
	//z positiva hacia atras. Para retrasar z ponemos 2.
	glm::mat4 visor = glm::lookAt(cam.position,cam.lookAt, cam.up);

	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 MVP = proyection * visor*trans;
	//pasarle matriz de traslaci�n al shader
	GLuint MVP_ID = glGetUniformLocation(programID, "MVP");

	//GLuint color_ID = glGetUniformLocation(programID, "vcolor");
	//glm::vec4 color = glm::vec4(1, 1, 0, 0);

	GLuint texSamplerID = glGetUniformLocation(programID, "sampler");

	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posici�n inicial del array.
	//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);//Para solo una textura no har�a falta esta l�nea. Para mas, s�. Hay que bindear cada vez.
	glUniform1i(texSamplerID,0);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);//introducimos variables del pol
	glDrawElements(GL_TRIANGLES, pol->vertexIndexCount, GL_UNSIGNED_INT, nullptr);

}
//void DrawTriangle2(polygon* pol, GLuint programID, camera_t cam, GLuint texID)
//{
//	glClear(GL_COLOR_BUFFER_BIT); //memory buffer reset
//
//	//Para dibujar desde GPU:
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	/*no deber�a ser necesario por que no hemos seleccionado ning�n otro objeto pero a�adimos por si acaso.
//	En caso de tener varios elementos habr�a que vincular el correspondiente al que queremos dibujar!*/
//	glBindVertexArray(pol->vertexArrayID);
//
//	glUseProgram(programID);
//
//	glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos1[0], pos[1], pos[2]));
//	glm::mat4  rot = glm::rotate(glm::mat4(1.0), rotAngle, glm::vec3(0, 1, 0));
//	trans = trans * rot;
//	//z positiva hacia atras. Para retrasar z ponemos 2.
//	glm::mat4 visor = glm::lookAt(cam.position, cam.lookAt, cam.up);
//
//	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//
//	glm::mat4 MVP = proyection * visor*trans;
//	//pasarle matriz de traslaci�n al shader
//	GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
//
//	//GLuint color_ID = glGetUniformLocation(programID, "vcolor");
//	//glm::vec4 color = glm::vec4(1, 1, 0, 0);
//
//	GLuint texSamplerID = glGetUniformLocation(programID, "sampler");
//
//	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posici�n inicial del array.
//	//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texID);//Para solo una textura no har�a falta esta l�nea. Para mas, s�. Hay que bindear cada vez.
//	glUniform1i(texSamplerID, 0);
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
//}

void MouseManager(GLFWwindow* win, double xpos, double ypos)
{
	pos[0] = (xpos / 640) - 0.5;
	pos[1] = (-ypos / 480) + 0.2;
}
camera_t cam;
void KeyBoardManager(GLFWwindow* win, int key, int scancode, int action, int mods)
{

	switch (key)
	{
		case GLFW_KEY_A:
		{
			//pos[0] -= 0.1;//pos[0] es x //antes de implementar la c�mara
			cam.position.x -= 0.1;
			cam.lookAt.x -= 0.1;//cuando nos movemos derecha/izquierda, mover el punto al que estamos mirando
			break;
		}
		case GLFW_KEY_D:
		{
			//pos[0] += 0.1;//pos[0] es x
			cam.position.x += 0.1;
			cam.lookAt.x += 0.1; //cuando nos movemos derecha / izquierda, mover el punto al que estamos mirando
			break;
		}
		case GLFW_KEY_W:
		{
			//pos[1] += 0.1;//pos[1] es y
			cam.position.z -= -0.1;
			cam.lookAt.z -= -0.1;//cuando nos movemos derecha/izquierda, mover el punto al que estamos mirando

			break;
		}
		case GLFW_KEY_S:
		{
			//pos[1] -= 0.1;//pos[1] es y
			cam.position.z += -0.1;
			cam.lookAt.z += -0.1;//cuando nos movemos derecha/izquierda, mover el punto al que estamos mirando
			break;
		}
		case GLFW_KEY_F:
		{
			rotAngle += 0.5;
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			exit(0);
		}
		default:
			break;
	}
}

void updateCamera(camera_t* cam, double* lastMX, double* lastMY, GLFWwindow* window)
{
	double mouseX, mouseY;
	double speedMX, speedMY;
	mouseX = mouseY = speedMX = speedMY = 0;//setear siempre variables que se declaran!!
	glfwGetCursorPos(window, &mouseX, &mouseY);
	speedMX = static_cast<int>(mouseX - *lastMX);
	speedMY = static_cast<int>(mouseY - *lastMY);

	*lastMX = mouseX; 
	*lastMY = mouseY;

	//speedMX esta en unidades de pixel y lookAt no.
	//en nuestro mundo las coodenadas no estan en pixeles
	//en concreto van desde -1 hasta 1 dice marcus
	//por eso  se divide velocidad entre 100.

	cam->lookAt.x += speedMX/100;//entre 100 para movernos despacio
	cam->lookAt.y += speedMY/100;//entre 100 para movernos despacio
}


/*argc y argv son argumentos para cuando hacemos aplicaciones de terminal.
Por si se ejecuta la app con nombre de la app + espacio + argumentos extra.
P.ej si le queires pasar alguna imagen inicial o alg�n par�metro de entrada
argc es el num de argumentos y argv puntero a char de car�cteres en el
que se describir�a cada argumento.
argc vale 1 y argv va a tener en primera posici�n el nombre del programa
y despues el resto de paramentros que quieras pasarle*/
int main(int argc, char** argv)
{
	glfwInit();
	//crear ventana
	GLFWwindow* win1 = glfwCreateWindow(640, 480,"Hola mundo glfw", nullptr,nullptr);
	//ponerla delante
	glfwMakeContextCurrent(win1);
	//no redimensionable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//callback de teclado. Ventana y funci�n como argumentos
	glfwSetKeyCallback(win1,KeyBoardManager);
	//callback de rat�n
	glfwSetCursorPosCallback(win1,MouseManager);

	glewInit();//hay que ponerlo despues de glfwInit();

	glEnable(GL_DEPTH_TEST);

	cam.lookAt = glm::vec3(0, 0, 0);
	cam.position = glm::vec3(0, 0, 2);//recordar que hacia atras es positivo
	cam.up = glm::vec3(0, 1, 0);
	double lastX, lastY;
	lastX = lastY = 0;

	//polygon* pol = createPolygon(); //Antes de implementar carga de xml. Una vez implementada:
	//polygon* pol = loadMSH("triangulo.msh");
	polygon* pol = loadMSH("cubo.msh");
	GLint programID = compileAndLinkShaderProgram(vertexShaderSRC, fragmentShaderSRC);
	uploadPolygonGPU(pol,programID);

	//GLuint texID = cargaTextura("data/top.png");
	GLuint texID = cargaTextura(pol->textureName);

	//mientras no cerrada {}. Tenemos que hacer glfwSwapBuffers(win1) al final para que se pinte en pantalla
	while (!(glfwWindowShouldClose(win1)))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //memory buffer reset
		updateCamera(&cam, &lastX, &lastY, win1);
		DrawTriangle(pol,programID,cam,texID);
		//DrawTriangle2(pol, programID, cam, texID);
		glfwSwapBuffers(win1);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


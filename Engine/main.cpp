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
#include "camera.h"
#include "object_t.h"
#include "light.h"

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
//	glm::mat4 visor = glm::lookAt(glm::vec3(0,0,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
//
//	glm::mat4 proyection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
//
//	glm::mat4 MVP = proyection * visor*trans;
//	//pasarle matriz de traslación al shader
//	GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
//
//	GLuint color_ID = glGetUniformLocation(programID, "vcolor");
//	//glm::vec4 color = glm::vec4(1, 1, 0, 0);
//
//	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posición inicial del array.
//	//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
//
//	glUniform4f(color_ID, 1.0f, 1.0f, 0, 0);
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
//
//}





//Implementamos objeto CAMERA

//void DrawTriangle2(polygon* pol, GLuint programID, camera_t cam, GLuint texID)
//{
//	glClear(GL_COLOR_BUFFER_BIT); //memory buffer reset
//
//	//Para dibujar desde GPU:
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	/*no debería ser necesario por que no hemos seleccionado ningún otro objeto pero añadimos por si acaso.
//	En caso de tener varios elementos habría que vincular el correspondiente al que queremos dibujar!*/
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
//	//pasarle matriz de traslación al shader
//	GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
//
//	//GLuint color_ID = glGetUniformLocation(programID, "vcolor");
//	//glm::vec4 color = glm::vec4(1, 1, 0, 0);
//
//	GLuint texSamplerID = glGetUniformLocation(programID, "sampler");
//
//	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);//[0][0] por que le pasamos el puntero a la posición inicial del array.
//	//el vector trans esta consecutivo en memoria pero nos podemos referir a el como array bidimensional
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texID);//Para solo una textura no haría falta esta línea. Para mas, sí. Hay que bindear cada vez.
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
			//pos[0] -= 0.1;//pos[0] es x //antes de implementar la cámara
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
		case GLFW_KEY_G:
		{
			//pos[1] += 0.1;//pos[1] es y
			cam.position.y -= -0.1;
			cam.lookAt.y -= -0.1;//cuando nos movemos derecha/izquierda, mover el punto al que estamos mirando

			break;
		}
		case GLFW_KEY_T:
		{
			//pos[1] -= 0.1;//pos[1] es y
			cam.position.y += -0.1;
			cam.lookAt.y += -0.1;//cuando nos movemos derecha/izquierda, mover el punto al que estamos mirando
			break;
		}
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
int main(int argc, char** argv)
{
	glfwInit();
	//crear ventana
	GLFWwindow* win1 = glfwCreateWindow(640, 480,"Hola mundo glfw", nullptr,nullptr);
	//ponerla delante
	glfwMakeContextCurrent(win1);
	//no redimensionable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//callback de teclado. Ventana y función como argumentos
	glfwSetKeyCallback(win1,KeyBoardManager);
	//callback de ratón
	glfwSetCursorPosCallback(win1,MouseManager);

	glewInit();//hay que ponerlo despues de glfwInit();

	glEnable(GL_DEPTH_TEST);

	cam.lookAt = glm::vec3(0, 0, 0);
	cam.position = glm::vec3(0, 0, 2);//recordar que hacia atras es positivo
	cam.up = glm::vec3(0, 1, 0);
	double lastX, lastY;
	lastX = lastY = 0;

	GLint programID = compileAndLinkShaderProgram(vertexShaderSRC, fragmentShaderSRC);
	//polygon* pol = createPolygon(); //Antes de implementar carga de xml. Una vez implementada:
	//polygon* pol = loadMSH("triangulo.msh");
	object_t* obj = createObject("data/asian_town.msh.xml", programID);
	
	//uploadPolygonGPU(pol,programID);//done now by createObject

	//GLuint texID = cargaTextura("data/top.png");
	//GLuint texID = cargaTextura(pol->textureName);


	//crate light. white color = (1,1,1)
	light_t* light = createLight(glm::vec3(1, 1, 1), 0.5, 0.5, glm::vec3(0, 0, 2));

	//mientras no cerrada {}. Tenemos que hacer glfwSwapBuffers(win1) al final para que se pinte en pantalla
	while (!(glfwWindowShouldClose(win1)))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //memory buffer reset
		updateCamera(&cam, &lastX, &lastY, win1);
		drawObject(obj, cam, programID, light);
		//DrawTriangle2(pol, programID, cam, texID);
		glfwSwapBuffers(win1);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


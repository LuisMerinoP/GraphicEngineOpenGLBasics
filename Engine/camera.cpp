#include "camera.h"

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

	cam->lookAt.x += speedMX / 100;//entre 100 para movernos despacio
	cam->lookAt.y += speedMY / 100;//entre 100 para movernos despacio
}
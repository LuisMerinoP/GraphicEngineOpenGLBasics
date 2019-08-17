#include "light.h"



light_t* createLight(glm::vec3 color, float ambientalStrength, float diffuseStrength, glm::vec3 pos)
{
	light_t* newLight = new light_t;
	newLight->color = color;
	newLight->ambientalStrength = ambientalStrength;
	newLight->diffuseStrength = diffuseStrength;
	newLight->pos = pos;

	return newLight;

}
void moveLight(light_t* light, glm::vec3 pos)
{
	light->pos = pos;
}

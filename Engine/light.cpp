#include "light.h"

light_t* createLight(glm::vec3 color, float ambStrength, float diffuseStrength, glm::vec3 pos)
{
	light_t* newlight = new light_t;

	newlight->color = color;
	newlight->ambientalStrength = ambStrength;
	newlight->diffuseStrength = diffuseStrength;
	newlight->pos = pos;

	return newlight;
}

void moveLight(light_t* light, glm::vec3 pos)
{
	light->pos = pos;
}
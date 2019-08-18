#include "emitter.h"
#include <glm/gtc/random.hpp>
#include <map>
emitter_t * createEmitter(const char * texture, GLuint programId,
	double lifeTimeMin, double lifeTimeMax,
	float spinMin, float spinMax,
	glm::vec3 speedMin, glm::vec3 speedMax,
	glm::vec3 scalingMin, glm::vec3 scalingMax,
	glm::vec3 position,
	float emissionRate)
{
	emitter_t *newEmi = new emitter_t;
	newEmi->particles = new std::list<particle_t*>();
	newEmi->lifeTime[0] = lifeTimeMin;
	newEmi->lifeTime[1] = lifeTimeMax;
	newEmi->spin[0] = spinMin;
	newEmi->spin[1] = spinMax;
	newEmi->speed[0] = speedMin;
	newEmi->speed[1] = speedMax;
	newEmi->scaling[0] = scalingMin;
	newEmi->scaling[1] = scalingMax;

	newEmi->initTime = glfwGetTime();
	newEmi->position = position;

	newEmi->emissionRate = emissionRate;
	newEmi->particlesToEmitAccum = 0;

	newEmi->texture = new char[strlen(texture) + 1];
	memcpy(newEmi->texture, texture, strlen(texture) + 1);

	return newEmi;
}


void updateEmitter(emitter_t* emi, GLuint programId)
{
	double actualTime = glfwGetTime();
	double deltaTime = actualTime - emi->initTime;

	double newParticlesCount = emi->emissionRate*deltaTime + emi->particlesToEmitAccum;
	int particlesToAdd = static_cast<int>(newParticlesCount) / 1;
	emi->particlesToEmitAccum = newParticlesCount - (double)particlesToAdd;


	for (int i = 0; i < particlesToAdd; i++)
	{
		particle_t* npart = createParticle(emi->texture, glm::linearRand(emi->lifeTime[0], emi->lifeTime[1]),
			glm::linearRand(emi->spin[0], emi->spin[1]),
			glm::linearRand(emi->speed[0], emi->speed[1]),
			programId);
		npart->bboard->position = emi->position;
		emi->particles->push_back(npart);
	}

	emi->initTime = actualTime;
}

void drawEmitter(emitter_t* emi, camera_t* cam, GLuint programID, light_t* light)
{

	double actualTime = glfwGetTime();
	double deltaTime = actualTime - emi->initTime;

	/*
	for (std::list<particle_t*>::iterator it = emi->particles->begin();
		it != emi->particles->end();
		++it)
	{
		updateSmokeParticle(*it,deltaTime);
		if ((*it)->lifeTime < 0)
		{
			particle_t* part = (*it);
			it = emi->particles->erase(it);
			delete part;
		}
	}
	*/

	std::map<float, particle_t*> sorted;
	std::list<particle_t*>::iterator i = emi->particles->begin();

	for (; i != emi->particles->end(); i++)
	{
		updateSmokeParticle((*i), deltaTime);
	}
	i = emi->particles->begin();
	int counter = 0;
	while (i != emi->particles->end())
	{
		float distance = cam->pos.z - (*i)->bboard->position.z;
		if ((*i)->lifeTime > 0)
		{
			sorted[counter] = (*i);
			++i;
		}
		else
		{
			particle_t* p = (*i);
			i = emi->particles->erase(i);
			delete p;
		}
		counter++;

	}

	for (std::list<particle_t*>::iterator it = emi->particles->begin();
		it != emi->particles->end();
		it++)
	{
		drawParticle(*it, cam, programID, light);
	}
}
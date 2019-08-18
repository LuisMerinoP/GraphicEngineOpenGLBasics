#pragma once
#include "particle.h"


typedef struct emitter_t
{
	std::list<particle_t*>* particles;
	double lifeTime[2];
	float spin[2];
	glm::vec3 speed[2];
	glm::vec3 scaling[2];
	double initTime;
	glm::vec3 position;

	double emissionRate;
	double particlesToEmitAccum;

	char* texture;
}emitter_t;

emitter_t * createEmitter(const char * texture, GLuint programId,
double lifeTimeMin, double lifeTimeMax,
float spinMin, float spinMax,
glm::vec3 speedMin, glm::vec3 speedMax,
glm::vec3 scalingMin, glm::vec3 scalingMax,
glm::vec3 position,
float emissionRate);

void updateEmitter(emitter_t* emi, GLuint programID);

void drawEmitter(emitter_t* emi, camera_t* cam, GLuint programID, light_t* light);


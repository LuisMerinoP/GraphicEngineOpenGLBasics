#pragma once
#include "object.h"


typedef struct particle_t
{
	object_t* bboard;
	double lifeTime;
	float spin;
	glm::vec3 speed;
}particle_t;

particle_t * createParticle(const char * texture, double lifeTime, float spin, glm::vec3 speed, GLuint programId);
void updateSmokeParticle(particle_t* part, float deltaTime);
void updateFlameParticle(particle_t* part, float deltaTime);
void drawParticle(particle_t* part, camera_t* cam, GLuint programID, light_t* light);
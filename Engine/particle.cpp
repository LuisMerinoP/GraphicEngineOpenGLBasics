#include "particle.h"
#include "billboard.h"

particle_t* createParticle(const char * texture, double lifeTime, float spin, glm::vec3 speed, GLuint programId) {
	particle_t* npart = new particle_t;

	npart->bboard = createBillboard(texture, programId);
	npart->lifeTime = lifeTime;
	npart->spin = spin;
	npart->speed = speed;
	
	return npart;
}
void updateSmokeParticle(particle_t* part, float deltaTime) {
	part->bboard->position += part->speed * deltaTime;
	part->lifeTime -= deltaTime;

	part->bboard->rotation.z += part->spin;
}
void updateFlameParticle(particle_t* part, float deltaTime) {

}
void drawParticle(particle_t* part, camera_t* cam, GLuint programID, light_t* light) {
	updateBillboard(part->bboard, cam);
	drawObject(part->bboard,*cam, programID, light);
}

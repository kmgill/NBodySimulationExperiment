/*
 * main.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */

#include "common.h"
#include "vector.h"
#include "particle.h"
#include "forceproviders.h"
#include "collisionproviders.h"
#include "leapfrogsimulator.h"
#include "solarsystemsnapshot.h"
#include <iostream>
#include <vector>

using namespace apoapsys;


Particle createParticleFromSnapshot(real * field) {
	Particle particle = Particle(field[FIELD_ID], field[FIELD_RADIUS], field[FIELD_MASS]);
	particle.position.set(field[FIELD_POS_X], field[FIELD_POS_Y], field[FIELD_POS_Z]);
	particle.velocity.set(field[FIELD_VEL_X], field[FIELD_VEL_Y], field[FIELD_VEL_Z]);

	particle.position.copyInto(particle.previousPosition);
	particle.velocity.copyInto(particle.previousVelocity);
	return particle;
}


void cleanCollisions(std::vector<Collision *> collisions) {
	while (collisions.size() > 0) {
		Collision * c = collisions[0];
		collisions.pop_back();
		PTRDELETE(c);
	}
}

int main(int argc, char ** argv) {

	std::cout << "Initializing Simulator Engine..." << std::endl;
	LeapFrogSimulator simulator = LeapFrogSimulator();
	//simulator.checkForCollisions(true);

	std::cout << "Adding general force providers..." << std::endl;
	NewtonianGravityForceProvider newtonForceProvider = NewtonianGravityForceProvider();
	simulator.addForceProvider(newtonForceProvider);

	std::cout << "Adding collision detectors..." << std::endl;
	DefaultCollisionDetectionProvider collisionProvider = DefaultCollisionDetectionProvider();
	simulator.addCollisionProvider(collisionProvider);

	std::cout << "Adding simulated bodies..." << std::endl;

	for (int b = 0; b < SNAPSHOT_BODIES; b++) {
		real * fields = SNAPSHOT[b];
		Particle particle = createParticleFromSnapshot(fields);
		simulator.addParticle(particle);
	}

	std::vector<Collision *> collisions;
	std::cout << "Running simulation..." << std::endl;

	for (long i = 0; i < 3.15569e7; i++) {
	//for (long i = 0; i < 1500000; i++) {
		simulator.step(1.0, collisions);

		if (collisions.size() > 0) {

			for (uint c = 0; c < collisions.size(); c++) {
				Collision * collision = collisions[c];
				std::cout << "Body #" << collision->particle0->identifier << " collided with body #" << collision->particle1->identifier << std::endl;
			}

			cleanCollisions(collisions);
		}

	}

	std::cout << "Done" << std::endl;

	return 0;
}





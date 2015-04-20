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
#include <omp.h>
#include <iostream>
#include <vector>
#include <time.h>

#ifdef WIN32
	#include <Windows.h>
#endif

using namespace apoapsys;

Particle particles[50];
int particleIndex;

Particle * createParticleFromSnapshot(real * field) {
	Particle * particle = new Particle(field[FIELD_ID], field[FIELD_RADIUS], field[FIELD_MASS]);
	//Particle * particle = &(particles[particleIndex]);
	//particle->identifier = field[FIELD_ID];
	//particle->radius = field[FIELD_RADIUS];
	//particle->mass = field[FIELD_MASS];
	//particleIndex++;

	particle->position.set(field[FIELD_POS_X], field[FIELD_POS_Y], field[FIELD_POS_Z]);
	particle->velocity.set(field[FIELD_VEL_X], field[FIELD_VEL_Y], field[FIELD_VEL_Z]);

	particle->position.copyInto(particle->previousPosition);
	particle->velocity.copyInto(particle->previousVelocity);
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
	

#ifdef WIN32
#ifdef NDEBUG
	//HANDLE process = GetCurrentProcess();
	//DWORD_PTR processAffinityMask = 1;
	//BOOL success = SetProcessAffinityMask(process, processAffinityMask);
	//std::cout << "Set CPU Affinity: " << success << std::endl;
#endif
#endif

	std::cout << "Num OpenMP Threads: " << omp_get_num_threads() << std::endl;
	std::cout << "Initializing Simulator Engine..." << std::endl;
	LeapFrogSimulator simulator = LeapFrogSimulator();
	//simulator.checkForCollisions(true);

	std::cout << "Adding general force providers..." << std::endl;
	NewtonianGravityForceProvider * newtonForceProvider = new NewtonianGravityForceProvider();
	simulator.addForceProvider(newtonForceProvider);

	std::cout << "Adding collision detectors..." << std::endl;
	DefaultCollisionDetectionProvider * collisionProvider = new DefaultCollisionDetectionProvider();
	//simulator.addCollisionProvider(collisionProvider);
	simulator.checkForCollisions(false);

	std::cout << "Adding simulated bodies..." << std::endl;

	Particle * earth = NULL;

	for (int b = 0; b < SNAPSHOT_BODIES; b++) {
		real * fields = SNAPSHOT[b];

		if (fields[FIELD_BODY_TYPE] == BODY_IS_MAJOR) {// || fields[FIELD_BODY_TYPE] == BODY_IS_MINOR) {
			Particle * particle = createParticleFromSnapshot(fields);
			simulator.addParticle(particle);

			if (particle->identifier == 1000300) {
				earth = particle;
			}
		}
	}

	std::vector<Collision *> collisions;
	std::cout << "Running simulation..." << std::endl;

	time_t t_start = time(NULL);

	long numIterations = 86400 * 365;// 3.15569e7;// 100000;

	Vector angleFrom(1.0, 0.0, 1.0);

	//for (long i = 0; i < 3.15569e7; i++) {
	for (long i = 0; i < numIterations; i++) {

		if (i % 1000 == 0) {
		//	std::cout << "Interation #" << i << ", Earth Distance: " << earth->position.length() << +", Velocity: " << earth->velocity.length() << ", Angle: " << DEGREES(angleFrom.angle(earth->position)) << std::endl;
		}
		
		simulator.step(1.0, collisions);

		if (earth != NULL) {
			//std::cout << "Earth Distance: " << earth->position.length() << + ", Velocity: " << earth->velocity.length() << std::endl;
		}

		if (collisions.size() > 0) {

			for (uint c = 0; c < collisions.size(); c++) {
				Collision * collision = collisions[c];
				std::cout << "Body #" << collision->particle0->identifier << " collided with body #" << collision->particle1->identifier << std::endl;
			}

			cleanCollisions(collisions);
		}

	}

	time_t t_end = time(NULL);

	std::cout << "Done in " << (t_end - t_start) << " seconds" << std::endl;

#ifdef WIN32
	system("PAUSE");
#endif

	return 0;
}




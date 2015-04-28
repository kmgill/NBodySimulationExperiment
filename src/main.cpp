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

// Basic configuration
#define NBODY_ITERATION_INTERVAL         1.0
#define NBODY_NUM_ITERATIONS             (86400 * 365 * 10)
#define NBODY_CHECK_COLLISIONS           false
#define NBODY_ALLOW_MAJOR_BODIES         true
#define NBODY_ALLOW_MINOR_BODIES         false
#define NBODY_ALLOW_MOONS                false
#define NBODY_SET_CPU_AFFINITY           false

#ifdef _DEBUG
#define NBODY_PAUSE_ON_COMPLETE          1
#else
#define NBODY_PAUSE_ON_COMPLETE          0
#endif

using namespace apoapsys;

namespace apoapsys {
	Particle * createParticleFromSnapshot(BodySnapshot * snapshot) {
		Particle * particle = new Particle(snapshot->name, snapshot->identifier, snapshot->radius, snapshot->mass);

		snapshot->position.copyInto(particle->position);
		snapshot->position.copyInto(particle->previousPosition);
		snapshot->velocity.copyInto(particle->velocity);
		snapshot->velocity.copyInto(particle->previousVelocity);

		return particle;
	}


	void cleanCollisions(std::vector<Collision *> * collisions) {
		while (!collisions->empty()) {
			Collision * c = collisions->at(0);
			collisions->erase(collisions->begin());
			delete c; 
		}
	}
};

int main(int argc, char ** argv) {


#ifdef WIN32
	if (NBODY_SET_CPU_AFFINITY) {
		HANDLE process = GetCurrentProcess();
		DWORD_PTR processAffinityMask = 1;
		BOOL success = SetProcessAffinityMask(process, processAffinityMask);
		std::cout << "Set CPU Affinity: " << success << std::endl;
	}
#endif

	std::cout << "Num OpenMP Threads: " << omp_get_num_threads() << std::endl;
	std::cout << "Initializing Simulator Engine..." << std::endl;
	LeapFrogSimulator simulator = LeapFrogSimulator();

	std::cout << "Adding general force providers..." << std::endl;
	NewtonianGravityForceProvider * newtonForceProvider = new NewtonianGravityForceProvider();
	simulator.addForceProvider(newtonForceProvider);

	std::cout << "Adding collision detectors..." << std::endl;
	DefaultCollisionDetectionProvider * collisionProvider = new DefaultCollisionDetectionProvider();
	simulator.addCollisionProvider(collisionProvider);
	simulator.checkForCollisions(NBODY_CHECK_COLLISIONS);

	std::cout << "Adding simulated bodies..." << std::endl;

	Particle * sun = NULL;
	Particle * earth = NULL;

	uint bodiesAdded = 0;
	for (int b = 0; b < apoapsys::snapshots.size(); b++) {
		BodySnapshot * snapshot = apoapsys::snapshots[b];
		if ((NBODY_ALLOW_MAJOR_BODIES && snapshot->bodyType == BODY_IS_MAJOR)
			|| (NBODY_ALLOW_MINOR_BODIES && snapshot->bodyType == BODY_IS_MINOR)
			|| (NBODY_ALLOW_MOONS && snapshot->bodyType == BODY_IS_MOON)) { 
			Particle * particle = createParticleFromSnapshot(snapshot);
			simulator.addParticle(particle);

			if (particle->identifier == 1000000) {
				sun = particle;
			}
			if (particle->identifier == 1000300) {
				earth = particle;
			}
			bodiesAdded++;
		}
	}
	std::cout << "Added " << bodiesAdded << " bodies" << std::endl;

	std::vector<Collision *> * collisions = new std::vector<Collision *>();
	std::cout << "Running simulation..." << std::endl;

	time_t t_start = time(NULL);


	for (long i = 0; i < NBODY_NUM_ITERATIONS; i++) {

		simulator.step(NBODY_ITERATION_INTERVAL, collisions);

		if (i % (86400) == 0) {
			real distAU = earth->position.distanceTo(sun->position) / 149597870700.0;
			std::cout << "Earth/Sun Distance: " << distAU << " AU, Earth Velocity: " << (earth->velocity.length() / 1000.0) << "km/s" << std::endl;
		}

		if (NBODY_CHECK_COLLISIONS && collisions->size() > 0) {

			for (uint c = 0; c < collisions->size(); c++) {
				Collision * collision = collisions->at(c);
				std::cout << "Body '" << collision->particle0->name << "' collided with body '" << collision->particle1->name << "' on iteration #" << i << std::endl;
			}

			cleanCollisions(collisions);
			PTRDELETE(collisions);
			collisions = new std::vector<Collision *>();
		}

	}

	time_t t_end = time(NULL);

	std::cout << "Done in " << (t_end - t_start) << " seconds" << std::endl;

#if NBODY_PAUSE_ON_COMPLETE
#ifdef WIN32
	system("PAUSE");
#endif
#endif

	return 0;
}




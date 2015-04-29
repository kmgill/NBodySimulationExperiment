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
#include "init.h"

#ifdef WIN32
	#include <Windows.h>
#endif

// Basic configuration
#define NBODY_ITERATION_INTERVAL         1.0
#define NBODY_NUM_ITERATIONS             (86400 * 7)
//* 365 * 1)
#define NBODY_CHECK_COLLISIONS           false
#define NBODY_ALLOW_MAJOR_BODIES         true
#define NBODY_ALLOW_MINOR_BODIES         true
#define NBODY_ALLOW_MOONS                true
#define NBODY_SET_CPU_AFFINITY           false

#ifdef _DEBUG
#define NBODY_PAUSE_ON_COMPLETE          1
#else
#define NBODY_PAUSE_ON_COMPLETE          0
#endif

using namespace apoapsys;


#if !USING_GTK_MAIN
int main(int argc, char ** argv) {


#ifdef WIN32
	if (NBODY_SET_CPU_AFFINITY) {
		HANDLE process = GetCurrentProcess();
		DWORD_PTR processAffinityMask = 1;
		BOOL success = SetProcessAffinityMask(process, processAffinityMask);
		std::cout << "Set CPU Affinity: " << success << std::endl;
	}
#endif

	std::cout << "Initializing Simulator Engine..." << std::endl;

	LeapFrogSimulator * simulator = createBasicLeapFrogSimulator(NBODY_ALLOW_MAJOR_BODIES, NBODY_ALLOW_MOONS, NBODY_ALLOW_MINOR_BODIES);

	std::vector<Collision *> * collisions = new std::vector<Collision *>();
	std::cout << "Running simulation..." << std::endl;

	time_t t_start = time(NULL);

	for (long i = 0; i < NBODY_NUM_ITERATIONS; i++) {

		simulator->step(NBODY_ITERATION_INTERVAL);

		if (i % (86400) == 0) {
			std::cout << "tick #" << (i / 86400) << std::endl;
		//	real distAU = earth->position.distanceTo(sun->position) / 149597870700.0;
		//	std::cout << "Earth/Sun Distance: " << distAU << " AU, Earth Velocity: " << (earth->velocity.length() / 1000.0) << "km/s" << std::endl;
		}

		if (NBODY_CHECK_COLLISIONS) {

			simulator->runCollisionDetection(collisions);
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

#endif


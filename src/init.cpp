/*
 * init.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: kgill
 */




#include "init.h"
#include "particle.h"
#include "solarsystemsnapshot.h"
#include "collisionproviders.h"
#include "leapfrogsimulator.h"
#include <vector>
#include <iostream>

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

	LeapFrogSimulator * createBasicLeapFrogSimulator(bool allowMajor, bool allowMoons, bool allowMinor) {
		LeapFrogSimulator * simulator = new LeapFrogSimulator();

		std::cout << "Adding general force providers..." << std::endl;
		NewtonianGravityForceProvider * newtonForceProvider = new NewtonianGravityForceProvider();
		simulator->addForceProvider(newtonForceProvider);

		std::cout << "Adding collision detectors..." << std::endl;
		DefaultCollisionDetectionProvider * collisionProvider = new DefaultCollisionDetectionProvider();
		simulator->addCollisionProvider(collisionProvider);

		std::cout << "Adding simulated bodies..." << std::endl;

		Particle * sun = NULL;
		Particle * earth = NULL;

		uint bodiesAdded = 0;
		for (int b = 0; b < apoapsys::snapshots.size(); b++) {
			BodySnapshot * snapshot = apoapsys::snapshots[b];
			if ((allowMajor && snapshot->bodyType == BODY_IS_MAJOR)
				|| (allowMinor && snapshot->bodyType == BODY_IS_MINOR)
				|| (allowMoons && snapshot->bodyType == BODY_IS_MOON)) {
				Particle * particle = createParticleFromSnapshot(snapshot);
				simulator->addParticle(particle);

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

		return simulator;
	}
};



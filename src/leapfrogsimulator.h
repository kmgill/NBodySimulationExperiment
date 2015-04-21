/*
 * leapfrogsimulator.h
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */

#ifndef LEAPFROGSIMULATOR_H_
#define LEAPFROGSIMULATOR_H_

#include "common.h"
#include "vector.h"
#include "particle.h"
#include "forceproviders.h"
#include "collisionproviders.h"

#include <vector>

namespace apoapsys {

	LIB_NBODY_API class LeapFrogSimulator {
	private:
		std::vector<Particle *> particles;
		std::vector<ForceProvider *> forceProviders;
		std::vector<CollisionDetectionProvider *> collisionProviders;

		bool _checkForCollisions;

		void stepVelocityVectors(real deltaT);
		void stepPositionVectors(real deltaT);
		Particle * checkForCollision(Particle * particle);

	public:
		LeapFrogSimulator();
		~LeapFrogSimulator();

		void checkForCollisions(bool c);
		void addParticle(Particle * particle);
		void addForceProvider(ForceProvider * forceProvider);
		void addCollisionProvider(CollisionDetectionProvider * collisionProvider);

		void step(real deltaT, std::vector<Collision *> * collisions = NULL);
	};

};



#endif /* LEAPFROGSIMULATOR_H_ */

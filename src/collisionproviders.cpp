/*
 * collisionproviders.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: kgill
 */

#include "common.h"
#include "particle.h"
#include "vector.h"
#include "collisionproviders.h"

namespace apoapsys {


	Collision::Collision(Particle * _particle0, Particle * _particle1)
		: particle0(_particle0)
		, particle1(_particle1){

	}

	bool DefaultCollisionDetectionProvider::checkCollision(Particle & p0, Particle & p1) {

		// Cannot collide with one's own self
		if (p0 == p1) {
			return false;
		}

		// Cannot collide with something that lacks volume
		if (p0.radius == 0.0 || p1.radius == 0.0) {
			return false;
		}

		real d = p0.position.distanceTo(p1.position);

		if (p0.radius + p1.radius > d / 1000.0) {
			return true;
		}

		return false;
	}
};


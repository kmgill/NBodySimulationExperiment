/*
 * collisionproviders.h
 *
 *  Created on: Feb 9, 2015
 *      Author: kgill
 */

#ifndef COLLISIONPROVIDERS_H_
#define COLLISIONPROVIDERS_H_

#include "common.h"
#include "particle.h"
#include "vector.h"

/*
 * public interface CollisionDetectionProvider {
	public boolean checkCollision(Particle p0, Particle p1);
}
 */

namespace apoapsys {

	LIB_NBODY_API class Collision{

	public:
		Particle * particle0;
		Particle * particle1;

		Collision(Particle * _particle0, Particle * _particle1);
		~Collision() {
			particle0 = NULL;
			particle1 = NULL;
		}

	};

	LIB_NBODY_API class CollisionDetectionProvider{

	public:
		CollisionDetectionProvider() { }
		virtual ~CollisionDetectionProvider() { }

		virtual bool checkCollision(Particle * p0, Particle * p1) = 0;

	};



	LIB_NBODY_API class DefaultCollisionDetectionProvider : public CollisionDetectionProvider{

	public:

		bool checkCollision(Particle * p0, Particle * p1);

	};

};


#endif /* COLLISIONPROVIDERS_H_ */

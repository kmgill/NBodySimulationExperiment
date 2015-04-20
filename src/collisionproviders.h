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

	class Collision {

	public:
		Particle * particle0;
		Particle * particle1;
		Particle merged;

		Collision(Particle * _particle0, Particle * _particle1);

	};

	class CollisionDetectionProvider {

	public:
		CollisionDetectionProvider() { }
		virtual ~CollisionDetectionProvider() { }

		virtual bool checkCollision(Particle * p0, Particle * p1) = 0;

	};



	class DefaultCollisionDetectionProvider : public CollisionDetectionProvider {

	public:

		bool checkCollision(Particle * p0, Particle * p1);

	};
	/*
	 * public class DefaultCollisionDetectionProviderImpl implements CollisionDetectionProvider {

	@Override
	public boolean checkCollision(Particle p0, Particle p1) {

		if (p0.equals(p1)) {
			return false; // Cannot collide with one's self
		}

		if (p0.body.getRadius() == 0.0 || p1.body.getRadius() == 0.0) {
			return false; // Cannot collide with something that has no volume
		}

		double d = p0.position.getDistanceTo(p1.position);
		if ((p0.body.getRadius() + p1.body.getRadius()) > d / 1000.0 ) {
			return true;
		}

		return false;
	}

}
	 */

};


#endif /* COLLISIONPROVIDERS_H_ */

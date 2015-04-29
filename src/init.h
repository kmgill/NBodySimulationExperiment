/*
 * init.h
 *
 *  Created on: Apr 29, 2015
 *      Author: kgill
 */

#include "particle.h"
#include "solarsystemsnapshot.h"
#include "collisionproviders.h"
#include "leapfrogsimulator.h"
#include <vector>

#pragma once

#ifndef INIT_H_
#define INIT_H_

namespace apoapsys {
	Particle * createParticleFromSnapshot(BodySnapshot * snapshot);
	void cleanCollisions(std::vector<Collision *> * collisions);

	LeapFrogSimulator * createBasicLeapFrogSimulator(bool allowMajor, bool allowMoons, bool allowMinor);
};



#endif /* INIT_H_ */

/*
 * forceproviders.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */


#include "common.h"
#include "particle.h"
#include "vector.h"
#include "forceproviders.h"
#include <vector>



namespace apoapsys {


	void NewtonianGravityForceProvider::onParticle(real deltaT, Particle * particle, std::vector<Particle *> * particles, Vector<real> * into) {
		particle->getAccelerationAtCurrentTime(particles, into);
	}


};




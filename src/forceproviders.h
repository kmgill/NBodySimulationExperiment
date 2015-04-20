/*
 * forceproviders.h
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */

#ifndef FORCEPROVIDERS_H_
#define FORCEPROVIDERS_H_

#include "common.h"
#include "particle.h"
#include "vector.h"
#include <vector>

namespace apoapsys {

	class ForceProvider {

	public:
		ForceProvider() { }
		virtual ~ForceProvider() { }
		virtual void onParticle(real deltaT, Particle * particle, std::vector<Particle *> * particles, Vector * into) = 0;

	};


	class NewtonianGravityForceProvider : public ForceProvider {

	public:
		void onParticle(real deltaT, Particle * particle, std::vector<Particle *> * particles, Vector * into);
	};


};

#endif /* FORCEPROVIDERS_H_ */

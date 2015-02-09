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
#include <vector>

namespace apoapsys {

	class LeapFrogSimulator {
	private:
		std::vector<Particle> particles;
		std::vector<ForceProvider> forceProviders;

		void stepVelocityVectors(real deltaT);
		void stepPositionVectors(real deltaT);
	public:
		LeapFrogSimulator();
		~LeapFrogSimulator();

		void addParticle(Particle & particle);
		void addForceProvider(ForceProvider & forceProvider);

		void step(real deltaT);
	};

};



#endif /* LEAPFROGSIMULATOR_H_ */

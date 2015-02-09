/*
 * leapfrogsimulator.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: kgill
 */


#include "common.h"
#include "mathext.h"
#include "vector.h"
#include "particle.h"
#include "forceproviders.h"
#include <vector>

#include "leapfrogsimulator.h"


namespace apoapsys {

	LeapFrogSimulator::LeapFrogSimulator() {

	}

	LeapFrogSimulator::~LeapFrogSimulator() {

	}


	void LeapFrogSimulator::addParticle(Particle & particle) {
		this->particles.push_back(particle);
	}

	void LeapFrogSimulator::addForceProvider(ForceProvider & forceProvider) {
		this->forceProviders.push_back(forceProvider);
	}

	void LeapFrogSimulator::stepVelocityVectors(real deltaT) {
		Vector acceleration;
		Vector accelSegment;

		// TODO: OpenMP
		for (uint p = 0; p < particles.size(); p++) {
			Particle particle = particles[p];
			acceleration.reset();
			for (uint fp = 0; fp < forceProviders.size(); fp++) {
				ForceProvider force = forceProviders[fp];
				force.onParticle(deltaT, particle, particles, accelSegment);
			}
			particle.velocity.copyInto(particle.previousVelocity);
			particle.velocity.x += deltaT * acceleration.x;
			particle.velocity.y += deltaT * acceleration.y;
			particle.velocity.z += deltaT * acceleration.z;

			particle.velocity.x = MIN(particle.velocity.x, _C);
			particle.velocity.y = MIN(particle.velocity.y, _C);
			particle.velocity.z = MIN(particle.velocity.z, _C);
		}
	}

	void LeapFrogSimulator::stepPositionVectors(real deltaT) {

		// TODO: OpenMP
		for (uint p = 0; p < particles.size(); p++) {
			Particle particle = particles[p];
			particle.position.copyInto(particle.previousPosition);
			particle.position.x += deltaT * particle.velocity.x;
			particle.position.y += deltaT * particle.velocity.y;
			particle.position.z += deltaT * particle.velocity.z;
		}
	}

	void LeapFrogSimulator::step(real deltaT) {
		stepVelocityVectors(deltaT);
		stepPositionVectors(deltaT);
	}
};

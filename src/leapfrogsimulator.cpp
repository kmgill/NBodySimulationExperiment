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
#include <omp.h>
#include <vector>

#include "leapfrogsimulator.h"


namespace apoapsys {

	LeapFrogSimulator::LeapFrogSimulator()
		: _checkForCollisions(false) {

	}

	LeapFrogSimulator::~LeapFrogSimulator() {

	}

	void LeapFrogSimulator::checkForCollisions(bool c) {
		this->_checkForCollisions = c;
	}

	void LeapFrogSimulator::addParticle(Particle & particle) {
		this->particles.push_back(particle);
	}

	void LeapFrogSimulator::addForceProvider(ForceProvider & forceProvider) {
		this->forceProviders.push_back(forceProvider);
	}

	void LeapFrogSimulator::addCollisionProvider(CollisionDetectionProvider & collisionProvider) {
		this->collisionProviders.push_back(collisionProvider);
	}

	void LeapFrogSimulator::stepVelocityVectors(real deltaT) {
		Vector acceleration;
		Vector accelSegment;

		// TODO: OpenMP

//#pragma omp parallel
//{
//		#pragma omp for nowait
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
//}
		//#pragma omp barrier

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

	Particle * LeapFrogSimulator::checkForCollision(Particle & particle) {
		if (this->collisionProviders.size() == 0) {
			return NULL;
		}

		if (particle.radius == 0.0) {
			return NULL;
		}

		for (uint p = 0; p < this->particles.size(); p++) {
			Particle other = this->particles[p];

			for (uint cp = 0; cp < this->collisionProviders.size(); cp++) {
				CollisionDetectionProvider collisionProvider = this->collisionProviders[cp];
				if (collisionProvider.checkCollision(particle, other)) {
					return &this->particles[p];
				}
			}
		}

		return NULL;
	}


	void LeapFrogSimulator::step(real deltaT, std::vector<Collision *> & collisions) {
		stepVelocityVectors(deltaT);
		stepPositionVectors(deltaT);

		if (this->_checkForCollisions) {


			for (uint p = 0; p < this->particles.size(); p++) {
				Particle particle = this->particles[p];
				Particle * collidesWith = this->checkForCollision(particle);

				// Remind the caller to clean these up on their time
				Collision * collision = new Collision(&particle, collidesWith);
				collisions.push_back(collision);
			}
		}

	}
};

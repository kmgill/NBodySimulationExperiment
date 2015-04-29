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
#include <iostream>

#include "leapfrogsimulator.h"


namespace apoapsys {

	LeapFrogSimulator::LeapFrogSimulator() {

	}

	LeapFrogSimulator::~LeapFrogSimulator() {

	}


	void LeapFrogSimulator::addParticle(Particle * particle) {
		this->particles.push_back(particle);
	}

	std::vector<Particle *> * LeapFrogSimulator::getParticles() {
		return &this->particles;
	}

	void LeapFrogSimulator::addForceProvider(ForceProvider * forceProvider) {
		this->forceProviders.push_back(forceProvider);
	}

	void LeapFrogSimulator::addCollisionProvider(CollisionDetectionProvider * collisionProvider) {
		this->collisionProviders.push_back(collisionProvider);
	}

	void LeapFrogSimulator::stepVelocityVectors(real deltaT) {
		
		
		size_t numParticles = particles.size();

#pragma omp for
		for (int p = 0; p < numParticles; p++) {
			Vector<real> acceleration;
			Vector<real> accelSegment;

			Particle * particle = particles[p];

			for (int fp = 0; fp < forceProviders.size(); fp++) {
				ForceProvider * force = forceProviders[fp];
				force->onParticle(deltaT, particle, &particles, &accelSegment);
				acceleration += accelSegment;
			}
			
			particle->velocity.copyInto(particle->previousVelocity);
			particle->velocity += (acceleration * deltaT);
		}


	}

	void LeapFrogSimulator::stepPositionVectors(real deltaT) {

		size_t numParticles = particles.size();
#pragma omp for
		for (int p = 0; p < numParticles; p++) {
			Particle * particle = particles[p];
			particle->position.copyInto(particle->previousPosition);
			particle->position += (particle->velocity * deltaT);
		}
	}

	Particle * LeapFrogSimulator::checkForCollision(Particle * particle) {

		if (!particle->enabled) {
			return NULL;
		}

		if (this->collisionProviders.size() == 0) {
			return NULL;
		}

		if (particle->radius == 0.0) {
			return NULL;
		}

		for (uint p = 0; p < this->particles.size(); p++) {
			Particle * other = this->particles[p];
			if (other->enabled) {
				for (uint cp = 0; cp < this->collisionProviders.size(); cp++) {
					CollisionDetectionProvider * collisionProvider = this->collisionProviders[cp];
					if (collisionProvider->checkCollision(particle, other)) {
						return this->particles[p];
					}
				}
			}
		}

		return NULL;
	}


	void LeapFrogSimulator::runCollisionDetection(std::vector<Collision *> * collisions) {
		size_t numParticles = this->particles.size();
		#pragma omp parallel for
		for (int p = 0; p < numParticles; p++) {
			Particle * particle = this->particles[p];
			Particle * collidesWith = this->checkForCollision(particle);

			if (collidesWith != NULL) {

				// This isn't exactly how it works, but for now...
				if (particle->mass > collidesWith->mass) {
					collidesWith->enabled = false;
					particle->mass += collidesWith->mass;
					particle->velocity += collidesWith->velocity;
				} else {
					particle->enabled = false;
					collidesWith->mass += particle->mass;
					collidesWith->velocity += particle->velocity;
				}

				Collision * collision = new Collision(particle, collidesWith);
				collisions->push_back(collision);
			}
		}

	}

	void LeapFrogSimulator::step(real deltaT) {
#pragma omp parallel
    {
		stepVelocityVectors(deltaT);
		stepPositionVectors(deltaT);
    }

	}
};

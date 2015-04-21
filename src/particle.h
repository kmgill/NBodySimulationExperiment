/*
 * particle.h
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */

#pragma once

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "common.h"
#include "mathext.h"
#include "vector.h"

#include <vector>
#include <string>


namespace apoapsys {

	LIB_NBODY_API class Particle {

	public:
		std::string name;
		int identifier;

		Vector<real> position;
		Vector<real> velocity;

		Vector<real> previousPosition;
		Vector<real> previousVelocity;

		real radius;
		real mass;

		bool enabled;

		Particle();
		Particle(std::string _name,  int _id, real _radius, real _mass);

		bool operator==(const Particle & o);
		bool operator!=(const Particle & o);
		real getKineticEnergy();
		real getPotentialEnergy(Particle * other);
		real getForceOnParticle(Particle * other);
		real getTotalForceOnParticle(std::vector<Particle *> * particles);
		real getLagrangianForceThreeBody(Particle * center, Particle * outter);
		real getGravitationalPotential(Particle * center);
		real getEstimatedOrbitalVelocity(Particle * center);
		real getCentrifugalForce(Particle * center);
		real getLegrangianFunction(Particle * center);
		void getForceVectorOnParticle(Particle * other, Vector<real> * into);
		void getForceVectorOnParticle(std::vector<Particle *> * particles, Vector<real> * into);
		void getAccelerationAtCurrentTime(Vector<real> * force, Vector<real> * into);
		void getAccelerationAtCurrentTime(std::vector<Particle *> * particles, Vector<real> * into);
		real getAngularMomentumSpinningBody();
		real getAngularMomentumOrbitingBody(Particle * centerParticle);
		real getAngularVelocity(Particle * centerParticle);
		real getEffectivePotentialEnergy(Particle * other);
		real getEffectivePotentialEnergy(std::vector<Particle *> * particles);
		real getEscapeVelocity(Particle * other);
		real getEscapeVelocity();
		//void determineGravitationBinding(std::vector<Particle> & particles);
		//void getMergedParticle(Particle & other, Particle & into);



	};


};


#endif /* PARTICLE_H_ */

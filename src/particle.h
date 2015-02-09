/*
 * particle.h
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "common.h"
#include "mathext.h"
#include "vector.h"

#include <vector>


namespace apoapsys {

	/*
	 * 	public String identifier;
	public SingleBody body;
	public Vector position;
	public Vector velocity;
	public Vector previousPosition = new Vector();
	public Vector previousVelocity = new Vector();
	 *
	 */
	class Particle {

	public:
		int identifier;

		Vector position;
		Vector velocity;

		Vector previousPosition;
		Vector previousVelocity;

		real radius;
		real mass;

		Particle(int _id, real _radius, real _mass);

		bool operator==(const Particle & o);
		bool operator!=(const Particle & o);
		real getKineticEnergy();
		real getPotentialEnergy(Particle & other);
		real getForceOnParticle(Particle & other);
		real getTotalForceOnParticle(std::vector<Particle> & particles);
		real getLagrangianForceThreeBody(Particle & center, Particle & outter);
		real getGravitationalPotential(Particle & center);
		real getEstimatedOrbitalVelocity(Particle & center);
		real getCentrifugalForce(Particle & center);
		real getLegrangianFunction(Particle & center);
		void getForceVectorOnParticle(Particle & other, Vector & into);
		void getForceVectorOnParticle(std::vector<Particle> & particles, Vector & into);
		void getAccelerationAtCurrentTime(Vector & force, Vector & into);
		void getAccelerationAtCurrentTime(std::vector<Particle> & particles, Vector & into);
		real getAngularMomentumSpinningBody();
		real getAngularMomentumOrbitingBody(Particle & centerParticle);
		real getAngularVelocity(Particle & centerParticle);
		real getEffectivePotentialEnergy(Particle & other);
		real getEffectivePotentialEnergy(std::vector<Particle> & particles);
		real getEscapeVelocity(Particle & other);
		real getEscapeVelocity();
		//void determineGravitationBinding(std::vector<Particle> & particles);
		//void getMergedParticle(Particle & other, Particle & into);



	};


};


#endif /* PARTICLE_H_ */

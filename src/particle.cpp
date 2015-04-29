/*
 * particle.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */



#include <vector>
#include <string>
#include "common.h"
#include "mathext.h"
#include "vector.h"
#include "particle.h"


namespace apoapsys {

	Particle::Particle()
		: identifier(0)
		, radius(0)
		, mass(0)
		, enabled(true) {

	}

	Particle::Particle(std::string _name, int _id, real _radius, real _mass)
		: name(_name)
		, identifier(_id)
		, radius(_radius)
		, mass(_mass)
		, enabled(true) {

	}

	bool Particle::operator==(const Particle & o) {
		return (this->identifier == o.identifier);
	}

	bool Particle::operator!=(const Particle & o) {
		return (this->identifier != o.identifier);
	}

	real Particle::getKineticEnergy() {
		return .5 * this->mass * SQR(this->velocity.length());

	}

	real Particle::getPotentialEnergy(Particle * other) {
		real r = this->position.distanceTo(other->position);
		real U = ((-_G * this->mass * other->mass) / r) + _K;
		return U;
	}

	real Particle::getForceOnParticle(Particle * other) {
		real d = other->position.distanceTo(this->position);
		real F = (_G * this->mass * other->mass) / SQR(d);
		return F;
	}

	real Particle::getTotalForceOnParticle(std::vector<Particle *> * particles) {
		Vector<real> F(0.0, 0.0, 0.0);
		Vector<real> f(0.0, 0.0, 0.0);

		for (uint i = 0; i < particles->size(); i++) {
			
			Particle * p = particles->at(i);

			if (this->identifier != p->identifier && p->enabled) {
				this->getForceVectorOnParticle(p, &f);
				F += f;
			}
		}

		real g = 1.0 / this->mass * F.length();
		return g;
	}

	real Particle::getLagrangianForceThreeBody(Particle * center, Particle * outter) {
		Vector<real> F(0.0, 0.0, 0.0);
		Vector<real> f;

		this->getForceVectorOnParticle(center, &f);
		F += f;

		this->getForceVectorOnParticle(outter, &f);
		F += f;

		this->getForceVectorOnParticle(center, &f);
		real Fc = f.length();
		double Fq = F.length();
		return Fq - Fc;
	}

	real Particle::getGravitationalPotential(Particle * center) {
		real m = this->mass;
		real M = center->mass;
		real r0 = this->previousPosition.distanceTo(center->previousPosition);
		real r = this->position.distanceTo(center->position);
		real tr = r - r0;
		real phi = this->getAngularVelocity(center);
		real E = .5 * m + ((tr * tr) + (r * r) * (phi * phi)) - ((_G * m * M) / r);
		return E;
	}

	real Particle::getEstimatedOrbitalVelocity(Particle * center) {
		real m = this->mass;
		real M = center->mass;
		real r = this->position.distanceTo(center->position);
		real Y = _G * (m + M);
		real X = r;
		real v = SQRT(Y / X);
		return v;
	}

	real Particle::getCentrifugalForce(Particle * center) {
		real r = this->position.distanceTo(center->position);
		real v = SQRT(SQR(this->velocity.x) + SQR(this->velocity.z));
		real w = r / v;
		real m = this->mass;
		real f = (m * SQR(w)) / r;
		return f;
	}

	real Particle::getLegrangianFunction(Particle * center) {
		real T = this->getKineticEnergy();
		real V = this->getPotentialEnergy(center);
		real L = T - V;
		return L;
	}

	void Particle::getForceVectorOnParticle(Particle * other, Vector<real> * into) {

		real d = other->position.distanceTo(this->position);
		real F = (_G * this->mass * other->mass) / (SQR(d));

		into->x = F * (other->position.x - this->position.x) / d;
		into->y = F * (other->position.y - this->position.y) / d;
		into->z = F * (other->position.z - this->position.z) / d;
	}

	void Particle::getForceVectorOnParticle(std::vector<Particle *> * particles, Vector<real> * into) {

		into->x = into->y = into->z = 0.0;
		Vector<real> forceFromOther;
		for (uint i = 0; i < particles->size(); i++) {
			Particle * other = particles->at(i);
			
			if (this->enabled && other->enabled && this->identifier != other->identifier) {
				
				this->getForceVectorOnParticle(other, &forceFromOther);
				
				*into += forceFromOther;
			}
			
		}

	}

	void Particle::getAccelerationAtCurrentTime(Vector<real> * force, Vector<real> * into) {
		into->set(force->x / this->mass, force->y / this->mass, force->z / this->mass);
	}

	void Particle::getAccelerationAtCurrentTime(std::vector<Particle *> * particles, Vector<real> * into) {
		Vector<real> forceOnParticles;
		this->getForceVectorOnParticle(particles, &forceOnParticles);
		this->getAccelerationAtCurrentTime(&forceOnParticles, into);
	}

	real Particle::getAngularMomentumSpinningBody() {
		real m = this->mass;
		real r = this->radius;
		real L = (2.0 / 5.0) * m * r * r * (2.0 * PI / 86400.0);
		return L;
	}

	real Particle::getAngularMomentumOrbitingBody(Particle * centerParticle) {
		real m = this->mass;
		real R = this->position.distanceTo(centerParticle->position);
		real l = m * SQR(R);
		real T = 0.1;
		real w = 2.0 * PI / T;
		real L = l * w;
		return L;
	}

	real Particle::getAngularVelocity(Particle * centerParticle) {
		real v = SQRT(SQR(this->velocity.x) + SQR(this->velocity.z));
		real r = this->position.distanceTo(centerParticle->position);
		real w = v / r;
		return w;
	}

	real Particle::getEffectivePotentialEnergy(Particle * other) {

		real r = this->position.distanceTo(other->position);
		real L = this->getAngularMomentumOrbitingBody(other);
		real m = this->mass;
		real M = other->mass;
		real eP = ((L * L) / (2.0 * m * (r * r))) - ((_G * m * M) / r);
		return eP;
	}

	real Particle::getEffectivePotentialEnergy(std::vector<Particle *> * particles) {
		real ttlEnergy = 0.0;
		for (uint i = 0; i < particles->size(); i++) {
			Particle * other = particles->at(i);
			if (this->identifier != other->identifier) {
				ttlEnergy += this->getEffectivePotentialEnergy(other);
			}
		}
		return 1.0 / this->mass * ttlEnergy;
	}

	real Particle::getEscapeVelocity(Particle * other) {
		real r = other->position.distanceTo(this->position);
		real v = SQRT((2 * _G * other->mass) / r);
		return v;
	}

	real Particle::getEscapeVelocity() {
		real ev = SQRT((2.0 * _G * this->mass) / this->radius);
		return ev;
	}

	/*
	void Particle::determineGravitationBinding(std::vector<Particle> & particles) {

	}

	void Particle::getMergedParticle(Particle & other, Particle & into) {

	}
	*/
};

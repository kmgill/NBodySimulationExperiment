/*
 * particle.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: kgill
 */




#include "common.h"
#include "mathext.h"
#include "vector.h"
#include "particle.h"
#include <vector>

namespace apoapsys {

	Particle::Particle()
		: identifier(0)
		, radius(0)
		, mass(0) {

	}

	Particle::Particle(int _id, real _radius, real _mass)
		: identifier(_id)
		, radius(_radius)
		, mass(_mass) {

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

	real Particle::getPotentialEnergy(Particle & other) {
		/*
		 * double m = body.getMass();
		double M = body.getMass();
		double r = position.getDistanceTo(other.position);
		double U =  ((-Constants.G * m * M) / r) + Constants.K;
		return U;
		 */
		real r = this->position.distanceTo(other.position);
		real U = ((-_G * this->mass * other.mass) / r) + _K;
		return U;
	}

	real Particle::getForceOnParticle(Particle & other) {
		/*double d = other.position.getDistanceTo(position);
		double F = (Constants.G * body.getMass() * other.body.getMass()) / (d * d);
		return F;
		*/
		real d = other.position.distanceTo(this->position);
		real F = (_G * this->mass * other.mass) / SQR(d);
		return F;
	}

	real Particle::getTotalForceOnParticle(std::vector<Particle> & particles) {
		/*
		 * Vector F = new Vector();
		for (Particle particle : particles) {
			if (!particle.equals(this)) {
				F.add(getForceVectorOnParticle(particle));
			}
		}


		double g = 1.0 / body.getMass() * F.length();
		return g;
		 */

		Vector F(0.0, 0.0, 0.0);
		Vector f(0.0, 0.0, 0.0);

		for (uint i = 0; i < particles.size(); i++) {
			Particle p = particles[i];

			if (*this != p) {
				this->getForceVectorOnParticle(p, f);
				F += f;
			}
		}

		real g = 1.0 / this->mass * F.length();
		return g;
	}

	real Particle::getLagrangianForceThreeBody(Particle & center, Particle & outter) {

		/*
		 * 		Vector F = new Vector();

		F.add(getForceVectorOnParticle(center));
		F.add(getForceVectorOnParticle(outter));


		double w = getAngularVelocity(center);

		double m = body.getMass();
		double r = position.getDistanceTo(center.position);

		Vector FcV = getForceVectorOnParticle(center);
		double Fc = FcV.length();
		double Fg = F.length();

		return Fg - Fc;
		 */

		Vector F(0.0, 0.0, 0.0);
		Vector f;

		this->getForceVectorOnParticle(center, f);
		F += f;

		this->getForceVectorOnParticle(outter, f);
		F += f;

		//real w = this->getAngularVelocity(center);
		//real m = this->mass;
		//real r = this->position.distanceTo(center.position);

		this->getForceVectorOnParticle(center, f);
		real Fc = f.length();
		double Fq = F.length();
		return Fq - Fc;
	}

	real Particle::getGravitationalPotential(Particle & center) {
		/*
		 * double m = body.getMass();
		double M = center.body.getMass();
		double r0 = previousPosition.getDistanceTo(center.previousPosition);
		double r = position.getDistanceTo(center.position);
		double tr = r - r0;
		double phi = this.getAngularVelocity(center);

		double E = .5 * m + ((tr * tr) + (r * r) * (phi * phi)) - ((Constants.G * m * M) / r);
		return E;
		 */

		real m = this->mass;
		real M = center.mass;
		real r0 = this->previousPosition.distanceTo(center.previousPosition);
		real r = this->position.distanceTo(center.position);
		real tr = r - r0;
		real phi = this->getAngularVelocity(center);
		real E = .5 * m + ((tr * tr) + (r * r) * (phi * phi)) - ((_G * m * M) / r);
		return E;
	}

	real Particle::getEstimatedOrbitalVelocity(Particle & center) {
		/*
		 * double m = body.getMass();
		double M = center.body.getMass();
		double r = position.getDistanceTo(center.position);

		double Y = Constants.G * (m + M);
		double X = r;
		double v = MathExt.sqrt(Y / X);

		return v;
		 */

		real m = this->mass;
		real M = center.mass;
		real r = this->position.distanceTo(center.position);
		real Y = _G * (m + M);
		real X = r;
		real v = SQRT(Y / X);
		return v;
	}

	real Particle::getCentrifugalForce(Particle & center) {
		/*
		 * 		double r = position.getDistanceTo(center.position);


		double v = MathExt.sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

		double w = r / v;

		double m = body.getMass();

		double f = (m * MathExt.sqr(w)) / r;
		return f;
		 */

		real r = this->position.distanceTo(center.position);
		real v = SQRT(SQR(this->velocity.x) + SQR(this->velocity.z));
		real w = r / v;
		real m = this->mass;
		real f = (m * SQR(w)) / r;
		return f;

	}

	real Particle::getLegrangianFunction(Particle & center) {
		/*
		 * double T = this.getKineticEnergy();
		double V = this.getPotentialEnergy(center);
		double L = T - V;
		return L;
		 */

		real T = this->getKineticEnergy();
		real V = this->getPotentialEnergy(center);
		real L = T - V;
		return L;
	}

	void Particle::getForceVectorOnParticle(Particle & other, Vector & into) {
		/*
		 * double dX = other.position.x - position.x;
		double dY = other.position.y - position.y;
		double dZ = other.position.z - position.z;
		double d = other.position.getDistanceTo(position);

		double F = (Constants.G * body.getMass() * other.body.getMass()) / (d * d);
		double fX0 = F * dX / d;
		double fY0 = F * dY / d;
		double fZ0 = F * dZ / d;

		into.x = fX0;
		into.y = fY0;
		into.z = fZ0;


		return into;
		 */

		real dX = other.position.x - this->position.x;
		real dY = other.position.y - this->position.y;
		real dZ = other.position.z - this->position.z;
		real d = other.position.distanceTo(this->position);

		real F = (_G * this->mass * other.mass) / (SQR(d));
		real fX0 = F * dX / d;
		real fY0 = F * dY / d;
		real fZ0 = F * dZ / d;

		into.x = fX0;
		into.y = fY0;
		into.z = fZ0;
	}

	void Particle::getForceVectorOnParticle(std::vector<Particle> & particles, Vector & into) {
		/*
		 * for (Particle other : particles) {

			if (this.equals(other)) {
				continue;
			}

			double d = this.position.getDistanceTo(other.position);
			if (d / 1000.0 < (this.body.getRadius() + other.body.getRadius())) {
				continue;
			}

			getForceVectorOnParticle(other, forceFromOther);

			into.x += forceFromOther.x;
			into.y += forceFromOther.y;
			into.z += forceFromOther.z;
		}

		return into;
		 */

		into.x = into.y = into.z = 0.0;

		for (uint i = 0; i < particles.size(); i++) {
			Particle other = particles[i];

			if (*this != other) {
				real d = this->position.distanceTo(other.position);
				if (d / 1000.0 < (this->radius + other.radius)) {
					continue;
				}

				Vector forceFromOther;
				this->getForceVectorOnParticle(other, forceFromOther);

				into += forceFromOther;
			}
		}

	}

	void Particle::getAccelerationAtCurrentTime(Vector & force, Vector & into) {
		/*
		 * 		double aX = force.x / body.getMass();
		double aY = force.y / body.getMass();
		double aZ = force.z / body.getMass();

		if (Double.isNaN(aX)) {
			aX = 0.0;
		}

		if (Double.isNaN(aY)) {
			aY = 0.0;
		}

		if (Double.isNaN(aZ)) {
			aZ = 0.0;
		}

		into.x = aX;
		into.y = aY;
		into.z = aZ;

		return into;
		 */



		into.x = force.x / this->mass;
		into.y = force.y / this->mass;
		into.z = force.z / this->mass;

	}

	void Particle::getAccelerationAtCurrentTime(std::vector<Particle> & particles, Vector & into) {
		/*
		 *
		getForceVectorOnParticle(particles, forceOnParticles);
		getAccelerationAtCurrentTime(forceOnParticles, into);

		return into;
		 */

		Vector forceOnParticles;
		this->getForceVectorOnParticle(particles, forceOnParticles);
		this->getAccelerationAtCurrentTime(forceOnParticles, into);
	}

	real Particle::getAngularMomentumSpinningBody() {
		/*
		 * double m = body.getMass();
		double r = body.getRadius();
		double L = (2.0 / 5.0) * m * r * r * (2.0 * Math.PI / 86400.0);
		return L;
		 */

		real m = this->mass;
		real r = this->radius;
		real L = (2.0 / 5.0) * m * r * r * (2.0 * PI / 86400.0);
		return L;
	}

	real Particle::getAngularMomentumOrbitingBody(Particle & centerParticle) {
		/*
		 * double m = body.getMass();
		double R = position.getDistanceTo(centerParticle.position);
		double l = m * MathExt.sqr(R);
		double T = 0.1;//365.25 * 24 * 60 * 60;
		double w = 2.0 * Math.PI / T;
		double L = l * w;
		return L;
		 */

		real m = this->mass;
		real R = this->position.distanceTo(centerParticle.position);
		real l = m * SQR(R);
		real T = 0.1;
		real w = 2.0 * PI / T;
		real L = l * w;
		return L;
	}

	real Particle::getAngularVelocity(Particle & centerParticle) {
		/*
		 * double v = MathExt.sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
		double r = position.getDistanceTo(centerParticle.position);
		double w = v / r;
		return w;
		 */

		real v = SQRT(SQR(this->velocity.x) + SQR(this->velocity.z));
		real r = this->position.distanceTo(centerParticle.position);
		real w = v / r;
		return w;
	}

	real Particle::getEffectivePotentialEnergy(Particle & other) {
		/*
		 * double r = this.position.getDistanceTo(other.position);

		double L = this.getAngularMomentumOrbitingBody(other);

		double m = this.body.getMass();
		double M = other.body.getMass();

		double effectivePotential = ((L * L) / (2.0 * m * (r * r))) - ((Constants.G * m * M) / r);

		return effectivePotential;
		 */

		real r = this->position.distanceTo(other.position);
		real L = this->getAngularMomentumOrbitingBody(other);
		real m = this->mass;
		real M = other.mass;
		real eP = ((L * L) / (2.0 * m * (r * r))) - ((_G * m * M) / r);
		return eP;
	}

	real Particle::getEffectivePotentialEnergy(std::vector<Particle> & particles) {
		/*
		 * 		double ttlEnergy = 0.0;

		for (Particle other : particles) {
			if (!other.equals(this)) {
				ttlEnergy += getEffectivePotentialEnergy(other);
			}
		}
		return 1.0 / body.getMass() * ttlEnergy;// / particles.length;
		 */

		real ttlEnergy = 0.0;
		for (uint i = 0; i < particles.size(); i++) {
			Particle other = particles[i];
			if (*this != other) {
				ttlEnergy += this->getEffectivePotentialEnergy(other);
			}
		}
		return 1.0 / this->mass * ttlEnergy;
	}

	real Particle::getEscapeVelocity(Particle & other) {
		/*
		 * double r = other.position.getDistanceTo(position);
		double v = MathExt.sqrt((2 * Constants.G * other.body.getMass()) / r);
		return v;
		 */

		real r = other.position.distanceTo(this->position);
		real v = SQRT((2 * _G * other.mass) / r);
		return v;
	}

	real Particle::getEscapeVelocity() {
		/*
		 * double ev = MathExt.sqrt((2.0 * Constants.G * body.getMass()) / body.getRadius());
		return ev;
		 */

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

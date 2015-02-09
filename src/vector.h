/*
 * vector.h
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */

#ifndef VECTOR_H_
#define VECTOR_H_


#include "common.h"

namespace apoapsys {

	class Vector {

	public:

		real x;
		real y;
		real z;
		real w;

		Vector();
		Vector(real _x, real _y, real _z, real _w = 0);

		void set(real _x, real _y, real _z);
		bool operator=(const Vector & o);
		Vector operator+(const Vector & o);
		Vector operator-(const Vector & o);
		Vector operator*(const Vector & o);
		Vector operator/(const Vector & o);

		Vector operator+=(const Vector & o);
		Vector operator-=(const Vector & o);
		Vector operator*=(const Vector & o);
		Vector operator*=(real multiple);
		Vector operator/=(const Vector & o);
		Vector operator/=(real divisor);

		real length();
		Vector copy();
		void copyInto(Vector & into);
		real distanceTo(const Vector & o);
		void inverse();
		void scale(real s);
		void normalize();
		Vector normalizedCopy();

		Vector unitVector();

		real dot(Vector & other, bool normalize = false);
		real dot4(Vector & other, bool normalize = false);
		real angle(Vector & other);
		Vector directionTo(Vector & other);
		Vector cross(const Vector & other);

		void reset();
	};



};

#endif /* VECTOR_H_ */

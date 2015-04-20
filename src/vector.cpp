/*
 * vector.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */


#include "common.h"
#include "vector.h"
#include "mathext.h"

namespace apoapsys {


	Vector::Vector()
		: x(0.0)
		, y(0.0)
		, z(0.0)
		, w(0.0) {


	}

	Vector::Vector(real _x, real _y, real _z, real _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w) {

	}

	void Vector::set(real _x, real _y, real _z) {
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	bool Vector::operator=(const Vector & o) {
		return (this->x == o.x && this->y == o.y && this->z == o.z && this->w == o.w);
	}

	Vector Vector::operator+(const Vector & o) {
		return Vector(this->x + o.x, this->y + o.y, this->z + o.z);
	}

	Vector Vector::operator-(const Vector & o) {
		return Vector(this->x - o.x, this->y - o.y, this->z - o.z);
	}

	Vector Vector::operator*(const Vector & o) {
		return Vector(this->x * o.x, this->y * o.y, this->z * o.z);
	}

	Vector Vector::operator/(const Vector & o) {
		return Vector(this->x / o.x, this->y / o.y, this->z / o.z);
	}

	Vector Vector::operator+=(const Vector & o) {
		this->x += o.x;
		this->y += o.y;
		this->z += o.z;
		this->w += o.w;
		return *this;
	}

	Vector Vector::operator-=(const Vector & o) {
		this->x -= o.x;
		this->y -= o.y;
		this->z -= o.z;
		this->w -= o.w;
		return *this;
	}

	Vector Vector::operator*=(const Vector & o) {
		this->x *= o.x;
		this->y *= o.y;
		this->z *= o.z;
		this->w *= o.w;
		return *this;
	}

	Vector Vector::operator*=(real multiple) {
		this->x *= multiple;
		this->y *= multiple;
		this->z *= multiple;
		this->w *= multiple;
		return *this;
	}

	Vector Vector::operator/=(const Vector & o) {
		this->x /= o.x;
		this->y /= o.y;
		this->z /= o.z;
		this->w /= o.w;
		return *this;
	}

	Vector Vector::operator/=(real divisor) {
		this->x /= divisor;
		this->y /= divisor;
		this->z /= divisor;
		this->w /= divisor;
		return *this;
	}

	real Vector::distanceTo(const Vector & o) {
		real x = this->x - o.x;
		real y = this->y - o.y;
		real z = this->z - o.z;
		return (SQRT(SQR(x) + SQR(y) + SQR(z)));
	}

	real Vector::length() {
		real len = (real) (SQRT(SQR(this->x) + SQR(this->y) + SQR(this->z)));
		return len;
	}

	Vector Vector::copy() {
		return Vector(this->x, this->y, this->z, this->w);
	}

	void Vector::copyInto(Vector & into) {
		into.x = this->x;
		into.y = this->y;
		into.z = this->z;
		into.w = this->w;
	}

	void Vector::inverse() {
		(*this) *= -1.0;
	}

	void Vector::scale(real s) {
		(*this) *= s;
	}

	void Vector::normalize() {
		real len = this->length();
		if (len == 0.0) {
			len = 1.0;
		}

		(*this) /= len;
	}

	Vector Vector::normalizedCopy() {
		Vector c = copy();
		c.normalize();
		return c;
	}

	Vector Vector::unitVector() {
		real len = this->length();
		if (len <= 0.0) {
			return Vector(0.0, 0.0, 0.0, 0.0);
		} else {
			return Vector(this->x / len, this->y / len, this->z / len, this->w / len);
		}
	}

	real Vector::dot(Vector & other, bool normalize) {
		Vector v0 = (normalize) ? this->normalizedCopy() : *this;
		Vector v1 = (normalize) ? other.normalizedCopy() : other;
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	real Vector::dot4(Vector & other, bool normalize) {
		Vector v0 = (normalize) ? this->normalizedCopy() : *this;
		Vector v1 = (normalize) ? other.normalizedCopy() : other;
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
	}

	real Vector::angle(Vector & other) {
		real dot = this->dot(other, true);
		real a = acos(CLAMP(dot, 1.0, -1.0));
		return a;
	}

	Vector Vector::directionTo(Vector & other) {
		Vector c = other.copy();
		c -= *this;
		c.normalize();
		return c;
	}

	Vector Vector::cross(const Vector & other) {
		real cX = this->y * other.z - other.y * this->z;
		real cY = this->z * other.x - other.z * this->x;
		real cZ = this->x * other.y - other.x * this->y;
		return Vector(cX, cY, cZ);
	}

	void Vector::reset() {
		this->x = this->y = this->z = this->w = 0.0;
	}
};


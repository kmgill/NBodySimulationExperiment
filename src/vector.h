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

	template<typename T>
	class Vector {

	public:

		T x;
		T y;
		T z;
		T w;

		Vector<T>()
			: x(0.0)
			, y(0.0)
			, z(0.0)
			, w(0.0) {
		}

		Vector<T>(T _x, T _y, T _z, T _w = 0.0)
			: x(_x)
			, y(_y)
			, z(_z)
			, w(_w) {

		}

		Vector<T> set(Vector<T> & other) {
			return this->set(other.x, other.y, other.z);
		}

		Vector<T> set(T _x, T _y, T _z) {
			this->x = _x;
			this->y = _y;
			this->z = _z;
			return *this;
		}

		bool operator=(const Vector<T> & o) {
			return (this->x == o.x && this->y == o.y && this->z == o.z && this->w == o.w);
		}

		Vector<T> operator+(const T v) {
			return Vector<T>(this->x + v, this->y + v, this->z + v);
		}

		Vector<T> operator+(const Vector<T> & o) {
			return Vector<T>(this->x + o.x, this->y + o.y, this->z + o.z);
		}

		Vector<T> operator-(const T v) {
			return Vector<T>(this->x - v, this->y - v, this->z - v);
		}

		Vector<T> operator-(const Vector<T> & o) {
			return Vector<T>(this->x - o.x, this->y - o.y, this->z - o.z);
		}

		Vector<T> operator*(const T v) {
			return Vector<T>(this->x * v, this->y * v, this->z * v);
		}

		Vector<T> operator*(const Vector<T> & o) {
			return Vector<T>(this->x * o.x, this->y * o.y, this->z * o.z);
		}

		Vector<T> operator/(const T v) {
			return Vector<T>(this->x / v, this->y / v, this->z / v);
		}

		Vector<T> operator/(const Vector<T> & o) {
			return Vector<T>(this->x / o.x, this->y / o.y, this->z / o.z);
		}

		Vector<T> operator+=(const Vector<T> & o) {
			this->x += o.x;
			this->y += o.y;
			this->z += o.z;
			this->w += o.w;
			return *this;
		}

		Vector<T> operator-=(const Vector<T> & o) {
			this->x -= o.x;
			this->y -= o.y;
			this->z -= o.z;
			this->w -= o.w;
			return *this;
		}

		Vector<T> operator*=(const Vector<T> & o) {
			this->x *= o.x;
			this->y *= o.y;
			this->z *= o.z;
			this->w *= o.w;
			return *this;
		}

		Vector<T> operator*=(T multiple) {
			this->x *= multiple;
			this->y *= multiple;
			this->z *= multiple;
			this->w *= multiple;
			return *this;
		}

		Vector<T> operator/=(const Vector<T> & o) {
			this->x /= o.x;
			this->y /= o.y;
			this->z /= o.z;
			this->w /= o.w;
			return *this;
		}

		Vector<T> operator/=(T divisor) {
			this->x /= divisor;
			this->y /= divisor;
			this->z /= divisor;
			this->w /= divisor;
			return *this;
		}

		T distanceTo(const Vector<T> & o) {
			T x = this->x - o.x;
			T y = this->y - o.y;
			T z = this->z - o.z;
			return (SQRT(SQR(x) + SQR(y) + SQR(z)));
		}

		T length() {
			T len = (T)(SQRT(SQR(this->x) + SQR(this->y) + SQR(this->z)));
			return len;
		}

		Vector<T> copy() {
			return Vector<T>(this->x, this->y, this->z, this->w);
		}

		void copyInto(Vector<T> & into) {
			into.x = this->x;
			into.y = this->y;
			into.z = this->z;
			into.w = this->w;
		}

		Vector<T> inverse() {
			(*this) *= -1.0;
			return *this;
		}

		Vector<T> scale(T s) {
			(*this) *= s;
			return *this;
		}

		Vector<T> normalize() {
			T len = this->length();
			if (len == 0.0) {
				len = 1.0;
			}

			(*this) /= len;

			return *this;
		}

		Vector<T> normalizedCopy() {
			Vector<T> c = copy();
			c.normalize();
			return c;
		}

		Vector<T> unitVector() {
			T len = this->length();
			if (len <= 0.0) {
				return Vector<T>(0.0, 0.0, 0.0, 0.0);
			}
			else {
				return Vector<T>(this->x / len, this->y / len, this->z / len, this->w / len);
			}
		}

		T dot(Vector<T> & other, bool normalize) {
			Vector<T> v0 = (normalize) ? this->normalizedCopy() : *this;
			Vector<T> v1 = (normalize) ? other.normalizedCopy() : other;
			return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
		}

		T dot4(Vector<T> & other, bool normalize) {
			Vector<T> v0 = (normalize) ? this->normalizedCopy() : *this;
			Vector<T> v1 = (normalize) ? other.normalizedCopy() : other;
			return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
		}

		T angle(Vector<T> & other) {
			T dot = this->dot(other, true);
			T a = acos(CLAMP(dot, 1.0, -1.0));
			return a;
		}

		Vector<T> directionTo(Vector<T> & other) {
			Vector<T> c = other.copy();
			c -= *this;
			c.normalize();
			return c;
		}

		Vector<T> cross(const Vector<T> & other) {
			T cX = this->y * other.z - other.y * this->z;
			T cY = this->z * other.x - other.z * this->x;
			T cZ = this->x * other.y - other.x * this->y;
			return Vector<T>(cX, cY, cZ);
		}

		Vector<T> reset() {
			this->x = this->y = this->z = this->w = 0.0;
			return *this;
		}

	};

};


LIB_NBODY_API 

#endif /* VECTOR_H_ */

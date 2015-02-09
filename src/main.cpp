/*
 * main.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */

#include "common.h"
#include "vector.h"

using namespace apoapsys;

int main(int argc, char ** argv) {

	Vector v0(1, 1, 1);
	Vector v1(2, 2, 2);

	Vector v3 = v0 + v1;
	Vector v4 = v0 * v1;

	v3 *= 2.0;
	v4 /= 2.0;

	return 0;
}





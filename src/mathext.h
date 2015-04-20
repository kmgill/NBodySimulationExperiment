/*
 * mathext.h
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */

#pragma once

#ifndef MATHEXT_H_
#define MATHEXT_H_


#include "common.h"
#include <math.h>


#define PI			3.14159265358979323846

#ifndef TWOPI
	#define TWOPI		6.283185307179586476925286766559
#endif

#ifndef HALFPI
	#define HALFPI		1.5707963267948966192313216916398
#endif

#ifndef FORTPI
	#define FORTPI		0.78539816339744830961566084581988
#endif

#ifndef EPS10
	#define EPS10		(1.e-10)
#endif

#ifndef RAD_90
	#define RAD_90		1.5707963267948966
#endif

#ifndef RAD_180
	#define RAD_180		3.14159265358979323846
#endif

#ifndef RAD_270
	#define RAD_270		4.71238898038469
#endif

#ifndef RAD_360
	#define RAD_360		360.0 * (PI / 180.0)
#endif

#ifndef AU_TO_KM
	#define AU_TO_KM 	149597870.700
#endif

#ifndef IAU_SECULAR_TERM_VALID_CENTURIES
	#define IAU_SECULAR_TERM_VALID_CENTURIES 50.0
#endif

#ifndef P03LP_VALID_CENTURIES
	#define P03LP_VALID_CENTURIES 5000.0
#endif

#ifndef _G
	#define _G 			6.67e-11
#endif

#ifndef _K
	#define _K 			0.0
#endif

#ifndef _C
	#define _C_ 		1.079253E9
#endif



#define C00			1.0
#define C02			0.25
#define C04			0.046875
#define C06			0.01953125
#define C08			0.01068115234375
#define C22			0.75
#define C44			0.46875
#define C46			0.01302083333333333333
#define C48			0.00712076822916666666
#define C66			0.36458333333333333333
#define C68			0.00569661458333333333
#define C88			0.3076171875




#ifndef MIN
	#define MIN(a, b)				((a) <= (b) ? (a) : (b))
#endif

#ifndef MAX
	#define MAX(a, b)				((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
	#define ABS(x)					(((x) > 0) ? (x) : ((x) * -1.0))
#endif

#define RADIANS(x)				((x) * 0.01745329251994329576923690768489)
#define DEGREES(x)				((x) * 57.295779513082320876798154814105)

#define CLAMP(x, high, low)		(MAX((low), (MIN((high), (x)))))
#define ROUND(x)				((int)((x) + 0.5))
#define ISBETWEEN(x, high, low)	((x) >= (low) && (x) <= (high))
#define SQR(x)					((x) * (x))
#define CUBE(x)					((x) * (x) * (x))

#define FLOOR(x)				((int) x) // Not really correct, I'm just being lazy at the moment...
#define CEIL(x)					((int) (x + 0.5)) // Not really correct, either, I'm just being lazy at the moment...


#define MAX3(a, b, c)			MAX(MAX(a, b), MAX(b, c))
#define MIN3(a, b, c)			MIN(MIN(a, b), MIN(b, c))


#define SQRT_MAGIC_F 0x5f3759df
#define SQRT_MAGIC_D 0x5fe6eb50c7b537a9


namespace apoapsys {
	//http://en.wikipedia.org/wiki/Fast_inverse_square_root
	INLINE float fast_sqrt_f(float x){
		float xhalf = 0.5f*x;
		union {
			float x;
			int i;
		} u;
		u.x = x;
		u.i = SQRT_MAGIC_F - (u.i >> 1);

		// This line can be repeated arbitrarily many times to increase accuracy 
		u.x = u.x * (1.5f - xhalf * u.x * u.x);
		u.x = u.x * (1.5f - xhalf * u.x * u.x); // twice should be sufficient for DemKit use
		//u.x = u.x * (1.5f - xhalf * u.x * u.x); // thrice should be sufficient for DemKit use

		return u.x * x;
	}

	//http://en.wikipedia.org/wiki/Fast_inverse_square_root
	INLINE real fast_sqrt_d(real x){
		real xhalf = 0.5*x;
		union {
			real x;
			long long i;
		} u;
		u.x = x;
		u.i = SQRT_MAGIC_D - (u.i >> 1);

		// This line can be repeated arbitrarily many times to increase accuracy 
		u.x = u.x * (1.5 - xhalf * u.x * u.x);
		u.x = u.x * (1.5 - xhalf * u.x * u.x); // twice should be sufficient for DemKit use
		//u.x = u.x * (1.5f - xhalf * u.x * u.x); // thrice should be sufficient for DemKit use

		return u.x * x;
	}
};



#define SQRT(x)				apoapsys::fast_sqrt_d(x)



#endif /* MATHEXT_H_ */

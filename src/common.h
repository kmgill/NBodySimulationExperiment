/*
 * common.h
 *
 *  Created on: Feb 7, 2015
 *      Author: kgill
 */

#ifndef COMMON_H_
#define COMMON_H_




#ifdef _MSC_VER
  #define INLINE __forceinline /* use __forceinline (VC++ specific) */
#else
  #define INLINE inline        /* use standard inline */
#endif

#ifndef WIN32

#include <inttypes.h>
#include <sys/syscall.h>
#include <sys/types.h>

typedef uint64_t __int64;
typedef uint32_t __int32;
typedef uint16_t __int16;
typedef uint8_t __int8;

#else 
typedef unsigned int uint;

#endif

#define PTRDELETE(x)							if (x != NULL) { delete(x); x = NULL; }
#define PTRDELETE_A(x)							if (x != NULL) { delete[](x); x = NULL; }

#define STRINGIZE_NX(A)							#A
#define STRINGIZE(A)							STRINGIZE_NX(A)

#ifndef NULL
#define NULL									0x0
#endif

#define FLOAT_MAX								3.4E+38
#define FLOAT_MIN								3.4E-38
#define DOUBLE_MAX								1.7E+308
#define DOUBLE_MIN								1.7E-308

#define REAL_MAX								DOUBLE_MAX
#define REAL_MIN								DOUBLE_MIN

typedef unsigned char uchar;
typedef unsigned char byte;

/*
typedef unsigned uint32_t uint;
typedef unsigned uint16_t ushort;

typedef __int32 int32;
typedef unsigned __int32 uint32;

typedef __int64 int64;
typedef unsigned __int64 uint64;
*/


//typedef signed __int64 real;
typedef double real;
#define _R(x)									real(x)

typedef byte byte2[2];
typedef byte byte3[3];
typedef byte byte4[4];
typedef byte byte8[8];

typedef float float2[2];
typedef float float3[3];
typedef float float4[4];
typedef float float5[5];
typedef float float6[6];
typedef float float7[7];
typedef float float8[8];

typedef double double2[2];
typedef double double3[3];
typedef double double4[4];
typedef double double5[5];
typedef double double6[6];
typedef double double7[7];
typedef double double8[8];

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];
typedef real real5[5];
typedef real real6[6];
typedef real real7[7];
typedef real real8[8];


#endif /* COMMON_H_ */

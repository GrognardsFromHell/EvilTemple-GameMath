
#if !defined(GAMEMATH_CONSTANTS_H)
#define GAMEMATH_CONSTANTS_H

#include "gamemath.h"

// Define several constants used by the intrinsic math methods
GAMEMATH_NAMESPACE_BEGIN

// A vector of positive infinity
extern const __m128 PositiveInfinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };

GAMEMATH_NAMESPACE_END

#endif // GAMEMATH_CONSTANTS_H

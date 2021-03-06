
#if !defined(GAMEMATH_CONSTANTS_H)
#define GAMEMATH_CONSTANTS_H

#include "gamemath_internal.h"

#include <limits>

// Define several constants used by the intrinsic math methods
GAMEMATH_NAMESPACE_BEGIN

#if !defined(GAMEMATH_NO_INTRINSICS)

// The first bit of an IEEE floating point number is the sign bit.
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int SignMask[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };

// Applies sign-bit masking only to the X, Y, and Z components
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int SignMaskXYZ[4] = { 0x80000000, 0x80000000, 0x80000000, 0x00000000 };

// All bits except the first bit (Sign Bit) are set. Can be used to make a floating point number absolute.
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int InvertedSignmask[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };

// The columns of the identity matrix.
GAMEMATH_ALIGN GAMEMATH_CONSTANT float IdentityCol1[4] = {1, 0, 0, 0};
GAMEMATH_ALIGN GAMEMATH_CONSTANT float IdentityCol2[4] = {0, 1, 0, 0};
GAMEMATH_ALIGN GAMEMATH_CONSTANT float IdentityCol3[4] = {0, 0, 1, 0};
GAMEMATH_ALIGN GAMEMATH_CONSTANT float IdentityCol4[4] = {0, 0, 0, 1};

// Factor used in construction of transformation matrices
GAMEMATH_ALIGN GAMEMATH_CONSTANT float FactorTwo[4] = {2, 2, 2, 2};

// A mask for the X coordinate of a Vector4
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int CoordinateMaskX[4] = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };

// A mask for the Y coordinate of a Vector4
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int CoordinateMaskY[4] = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };

// A mask for the Z coordinate of a Vector4
GAMEMATH_ALIGN GAMEMATH_CONSTANT unsigned int CoordinateMaskZ[4] = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };

// Positive infinity
GAMEMATH_ALIGN GAMEMATH_CONSTANT float PositiveInfinity[4] = { std::numeric_limits<float>::infinity(), 
                                                                                std::numeric_limits<float>::infinity(), 
                                                                                std::numeric_limits<float>::infinity(), 
                                                                                std::numeric_limits<float>::infinity() };
// Negative infinity
GAMEMATH_ALIGN GAMEMATH_CONSTANT float NegativeInfinity[4] = { -std::numeric_limits<float>::infinity(), 
                                                                                -std::numeric_limits<float>::infinity(), 
                                                                                -std::numeric_limits<float>::infinity(), 
                                                                                -std::numeric_limits<float>::infinity() };

#endif

GAMEMATH_NAMESPACE_END

#endif // GAMEMATH_CONSTANTS_H

#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <new>

#define GAMEMATH_NAMESPACE_BEGIN namespace GameMath {
#define GAMEMATH_NAMESPACE_END }

#if defined(_MSC_VER)
#  define ALIGNED_MALLOC(size) _aligned_malloc(size, 16)
#  define ALIGNED_FREE(ptr) _aligned_free(ptr)
#  define GAMEMATH_ALIGN __declspec(align(16))
#  define GAMEMATH_INLINE __forceinline
#elif defined(__GNUC__)
#include <mm_malloc.h>
#  define ALIGNED_MALLOC(size) _mm_malloc(size, 16)
#  define ALIGNED_FREE(ptr) _mm_free(ptr)
#  define GAMEMATH_ALIGN __attribute__((aligned(16)))
#  define GAMEMATH_INLINE inline
#else
#  warning "Falling back to malloc and unaligned data. This may lead to runtime crashes."
#  define ALIGNED_MALLOC(size) malloc(size)
#  define ALIGNED_FREE(ptr) free(ptr)
#  define GAMEMATH_ALIGN
#  define GAMEMATH_INLINE inline
#endif

#if !defined(GAMEMATH_NO_INTRINSICS)
#include <emmintrin.h>
#endif

// Override new and delete to force 16-byte alignment on all platforms
inline void* operator new(size_t size)
{
	void *result = ALIGNED_MALLOC(size);
	if (!result)
		throw std::bad_alloc();
	return result;
}

inline void operator delete(void *ptr)
{
	ALIGNED_FREE(ptr);
}

#include "gamemath_constants.h"
#include "vector4.h"
#include "matrix4.h"

#endif // GAMEMATH_H

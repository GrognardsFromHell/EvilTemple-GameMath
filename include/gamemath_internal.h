
#ifndef GAMEMATH_INTERNAL_H
#define GAMEMATH_INTERNAL_H

#include <new>

#define GAMEMATH_NAMESPACE_BEGIN namespace GameMath {
#define GAMEMATH_NAMESPACE_END }

#if defined(_MSC_VER)
#  define ALIGNED_MALLOC(size) _aligned_malloc(size, 16)
#  define ALIGNED_FREE(ptr) _aligned_free(ptr)
#  define GAMEMATH_ALIGN __declspec(align(16))
#  define GAMEMATH_INLINE __forceinline
#  define GAMEMATH_CONSTANT extern const __declspec(selectany)
#  define GAMEMATH_ALIGNEDTYPE_PRE GAMEMATH_ALIGN
#  define GAMEMATH_ALIGNEDTYPE_MID 
#  define GAMEMATH_ALIGNEDTYPE_POST 
#elif defined(__GNUC__)
#include <mm_malloc.h>
#  define ALIGNED_MALLOC(size) _mm_malloc(size, 16)
#  define ALIGNED_FREE(ptr) _mm_free(ptr)
#  define GAMEMATH_ALIGN __attribute__((aligned(16)))
#  define GAMEMATH_INLINE inline
#  define GAMEMATH_CONSTANT static const
#  define GAMEMATH_ALIGNEDTYPE_PRE 
#  define GAMEMATH_ALIGNEDTYPE_MID 
#  define GAMEMATH_ALIGNEDTYPE_POST GAMEMATH_ALIGN
#else
#  warning "Falling back to malloc and unaligned data. This may lead to runtime crashes."
#  define ALIGNED_MALLOC(size) malloc(size)
#  define ALIGNED_FREE(ptr) free(ptr)
#  define GAMEMATH_ALIGN
#  define GAMEMATH_INLINE inline
#  define GAMEMATH_CONSTANT static const
#  define GAMEMATH_ALIGNED_CLASS class
#  define GAMEMATH_ALIGNEDTYPE_PRE 
#  define GAMEMATH_ALIGNEDTYPE_MID 
#  define GAMEMATH_ALIGNEDTYPE_POST
#endif

#if !defined(GAMEMATH_NO_INTRINSICS)
#include <emmintrin.h>
#endif

// Override new and delete to force 16-byte alignment on all platforms
#if !defined(GAMEMATH_NO_MEMORY_OPERATORS)
GAMEMATH_INLINE void* operator new(size_t size)
{
	void *result = ALIGNED_MALLOC(size);
	if (!result)
		throw std::bad_alloc();
	return result;
}

GAMEMATH_INLINE void operator delete(void *ptr)
{
	ALIGNED_FREE(ptr);
}

GAMEMATH_INLINE void* operator new[](size_t size)
{
	void *result = ALIGNED_MALLOC(size);
	if (!result)
		throw std::bad_alloc();
	return result;
}

GAMEMATH_INLINE void operator delete[](void *ptr)
{
	ALIGNED_FREE(ptr);
}
#endif // GAMEMATH_NO_MEMORY_OPERATORS

GAMEMATH_NAMESPACE_BEGIN
class Vector4;
class Matrix4;
GAMEMATH_NAMESPACE_END

#include "gamemath_constants.h"
    
#endif // GAMEMATH_INTERNAL_H

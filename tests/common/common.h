#if !defined(COMMON_H)
#define COMMON_H

#include "../../include/gamemath.h"

#include <vector>
#include <algorithm>
#include <limits>

typedef unsigned int uint;

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#error "Measurements not yet supported on other platforms."
#endif

class Timer {
public:
	Timer();
	void start();
	double end();
private:
	LARGE_INTEGER mStart;
	LARGE_INTEGER mFrequency;
};

GAMEMATH_INLINE Timer::Timer() 
{
	QueryPerformanceFrequency(&mFrequency);
}

GAMEMATH_INLINE void Timer::start() 
{
	QueryPerformanceCounter(&mStart);
}

GAMEMATH_INLINE double Timer::end()
{
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);

	return (endTime.QuadPart - mStart.QuadPart) * 1000 / (double)mFrequency.QuadPart;
}

class BenchmarkIterator {
public:
	BenchmarkIterator(const char *description);
	~BenchmarkIterator();
	void next();
	bool atEnd();
private:
	const char *mDescription;
	Timer timer;
	std::vector<double> mTimings;
	long mIteration;
	long mTotalIterations;
};

GAMEMATH_INLINE BenchmarkIterator::BenchmarkIterator(const char *description) 
	: mDescription(description), mIteration(0), mTotalIterations(10000) {
	timer.start();
}

GAMEMATH_INLINE BenchmarkIterator::~BenchmarkIterator() {
	// Median over a list of values
	std::sort(mTimings.begin(), mTimings.end(), std::less<double>());

	double perIteration = mTimings.at(mTimings.size() / 2);

	printf("%s Iteration: %Lf ms (Total: %Ld)\n", mDescription, perIteration, mIteration);
}

GAMEMATH_INLINE void BenchmarkIterator::next() {
	mTimings.push_back(timer.end());
	++mIteration;
	timer.start();
}

GAMEMATH_INLINE bool BenchmarkIterator::atEnd() {
	return mIteration >= mTotalIterations;
}

#define BENCHMARK(description) for (BenchmarkIterator __iterator(description); !__iterator.atEnd(); __iterator.next())

GAMEMATH_INLINE void aCompare(float actual, float expected, const char *actualExpression, const char *expectedExpression) {
	float difference = actual - expected;

	if (difference <= std::numeric_limits<float>::epsilon() && difference >= - std::numeric_limits<float>::epsilon()) {
		return;
	}

	printf("Comparison failed for %s = %s, %.10f != %.10f.\n", actualExpression, expectedExpression, actual, expected);
}

#define COMPARE(actual, expected) aCompare(actual, expected, #actual, #expected);

static void printVector(const GameMath::Vector4 &v) {
	printf("%f %f %f %f\n", v.x(), v.y(), v.z(), v.w());	
}

#endif // COMMON_H

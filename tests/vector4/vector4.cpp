
#include "../../include/gamemath.h"

#include <limits>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace GameMath;

static Vector4 result[10000];

int main(int argc, char *argv[])
{
	Vector4 vec(1, 2, 3, 4);

	printf("%f %f %f %f\n", vec.x(), vec.y(), vec.z(), vec.w());

	printf("Length Squared: %f.\n", vec.lengthSquared());
	printf("Length: %f.\n", vec.length());

	vec.normalize();
	printf("%f %f %f %f\n", vec.x(), vec.y(), vec.z(), vec.w());

	double totalElapsed = 0;
	const int totalRuns = 10000;

	for (int j = 0; j < totalRuns; ++j) {
		LARGE_INTEGER start, end;
		QueryPerformanceCounter(&start);

		for (int i = 0; i < 10000; ++i) {
			Vector4 a(0, 0, 1, 0);
			Vector4 b(0, 1, 0, 0);
			result[i] = b.cross(a);
		}

		QueryPerformanceCounter(&end);

		double elapsed = (double)(end.QuadPart - start.QuadPart);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		totalElapsed += elapsed / (double)frequency.QuadPart;
	}

	printf("Elapsed ms: %Lf\n", (double)(totalElapsed * 1000 / (double)totalRuns));

	Vector4 c = - result[0];
	printf("%f %f %f %f\n", c.x(), c.y(), c.z(), c.w());

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

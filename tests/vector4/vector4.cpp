
#define GAMEMATH_NO_INTRINSICS
#include "../common/common.h"

#include <limits>
#include <cstdio>

using namespace GameMath;

static Vector4 result[10000];

int main(int argc, char *argv[])
{
	Vector4 vec(1, 2, 3, 4);

	printf("%f %f %f %f\n", vec.x(), vec.y(), vec.z(), vec.w());

	COMPARE(vec.lengthSquared(), 30);
	COMPARE(vec.length(), std::sqrtf(30));

	vec.normalize();
	COMPARE(vec.lengthSquared(), 1);
	COMPARE(vec.length(), 1);

	BENCHMARK("Vector cross product & normalization") {
		for (int i = 0; i < 10000; ++i) {
			Vector4 a(0, 0, 1, 0);
			Vector4 b(0, 1, 0, 0);
			result[i] = b.cross(a).normalize();
		}
	}
	
	Vector4 c = - result[0];
	printf("%f %f %f %f\n", c.x(), c.y(), c.z(), c.w());

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

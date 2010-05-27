
#include "../common/common.h"

#include <limits>
#include <cstdio>

using namespace GameMath;

static Vector4 result[1000];

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
		for (int i = 0; i < 1000; ++i) {
			Vector4 a(0, 0, 1, 0);
			Vector4 b(0, 1, 0, 0);
			result[i] = b.cross(a).normalize();
		}
	}

	Vector4 scaleTest(1, 2, 3, 4);
	Vector4 resVec = 0.5 * scaleTest;
	COMPARE(resVec.x(), 0.5f);
	COMPARE(resVec.y(), 1.0f);
	COMPARE(resVec.z(), 1.5f);
	COMPARE(resVec.w(), 4.0f);
	
	resVec = scaleTest * 0.5f;
	COMPARE(resVec.x(), 0.5f);
	COMPARE(resVec.y(), 1.0f);
	COMPARE(resVec.z(), 1.5f);
	COMPARE(resVec.w(), 4.0f);

	resVec = scaleTest;
	resVec *= 0.5f;
	COMPARE(resVec.x(), 0.5f);
	COMPARE(resVec.y(), 1.0f);
	COMPARE(resVec.z(), 1.5f);
	COMPARE(resVec.w(), 4.0f);
    
	Vector4 c = - result[0];
	printf("%f %f %f %f\n", c.x(), c.y(), c.z(), c.w());

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

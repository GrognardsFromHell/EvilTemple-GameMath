
#define _USE_MATH_DEFINES
#include <math.h>

#include <time.h>

#include "../common/common.h"

using namespace GameMath;

static Vector4 positionIn[10000];
static Vector4 normalIn[10000];
static Vector4 positionOut[10000];
static Vector4 normalOut[10000];

int main(int argc, char *argv[])
{
	Matrix4 m = Matrix4::scaling(2, 2, 2);

	Vector4 v(1, 2, 3, 4);
	Vector4 c = m * v;

	srand(time(NULL));
	for (int i = 0; i < 10000; ++i) {
		positionIn[i] = Vector4(rand(), rand(), rand(), rand());
		normalIn[i] = Vector4(rand(), rand(), rand(), rand()).normalize();
	}

	printVector(c);
	
	BENCHMARK("Iterate over 10000 vectors and multiply them with a matrix.") {		
		for (int i = 0; i < 1000; ++i) {
			positionOut[i] = m.mapPosition(positionIn[i]);
			normalOut[i] = m.mapNormal(normalIn[i]);
		}
	}

	Quaternion rotation = Quaternion::fromAxisAndAngle(0, 1, 0, (float)M_PI_2); // rotation of 90° around the Y axis

	Matrix4 boneMatrix = Matrix4::transformation(Vector4(2, 3, 4, 0), rotation, Vector4(10, 20, 30, 0));

	Vector4 result = boneMatrix * Vector4(1, 0, 0, 1);
	COMPARE(result.x(), 10);
	COMPARE(result.y(), 20);
	COMPARE(result.z(), 32);
	COMPARE(result.w(), 1);

	Matrix4 trans1 = Matrix4::translation(1, 2, 3, 4);
	COMPARE(trans1(0, 3), 1);
	COMPARE(trans1(1, 3), 2);
	COMPARE(trans1(2, 3), 3);
	COMPARE(trans1(3, 3), 5); // identity matrix is base

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

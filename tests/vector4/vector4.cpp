
#include "../../include/gamemath.h"

#include <limits>
#include <cstdio>

using namespace GameMath;

int main(int argc, char *argv[])
{
	Vector4 vec(1, 2, 3, 4);

	printf("%f %f %f %f\n", vec.x(), vec.y(), vec.z(), vec.w());

	printf("Length Squared: %f.\n", vec.lengthSquared());
	printf("Length: %f.\n", vec.length());

	vec.normalize();
	printf("%f %f %f %f\n", vec.x(), vec.y(), vec.z(), vec.w());

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}


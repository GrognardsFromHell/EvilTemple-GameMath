
#include "../common/common.h"

using namespace GameMath;

int main(int argc, char *argv[])
{
	Matrix4 m = Matrix4::scaling(2, 2, 2);

	Vector4 v(1, 2, 3, 4);
	Vector4 c = m * v;

	printVector(c);

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

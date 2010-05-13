
#define GAMEMATH_NO_INTRINSICS
#include "../common/common.h"

#include <limits>
#include <cstdio>

using namespace GameMath;

int main(int argc, char *argv[])
{
	Box2d box(1.1f, 2.2f, 3.3f, 4.4f);

	COMPARE(box.left(), 1.1f);
	COMPARE(box.top(), 2.2f);
	COMPARE(box.right(), 3.3f);
	COMPARE(box.bottom(), 4.4f);

	// Auto-Swap test
	Box2d unswappedBox(3, 4, 1, 2);

	COMPARE(box.left(), 1.1f);
	COMPARE(box.top(), 2.2f);
	COMPARE(box.right(), 3.3f);
	COMPARE(box.bottom(), 4.4f);

	// Intersection test
	Box2d box1(-1, -1, 1, 1);
	Box2d box2(-0.5f, -0.5f, 0.5f, 0.5f); // Contained within box 1
	EXPECT(box1.intersects(box2));
	EXPECT(box2.intersects(box1));
	
	
	Box2d box3(-2, -2, -1, 2); // To the left of box1, surfaces touch, but don't intersect
	EXPECT(!box1.intersects(box3));
	EXPECT(!box3.intersects(box1));
	
	Box2d box4(-2, -2, -0.9f, 2); // To the left of box1, partial intersection
	EXPECT(box1.intersects(box4));
	EXPECT(box4.intersects(box1));

	printf("Press enter to continue.\n");
	fgetc(stdin);

	return 0;
}

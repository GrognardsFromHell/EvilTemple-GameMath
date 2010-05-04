
#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "model.h"

#include "../../include/gamemath.h"
using namespace GameMath;

GLUquadric *sphere;

static Vector4 positions[8] = {
	// Bottom
	Vector4(-1, -1, -1, 1), // Front, Left
	Vector4(1, -1, -1, 1), // Front, Right
	Vector4(1, -1, 1, 1), // Back, Right
	Vector4(-1, -1, 1, 1), // Back, Left
	// Top
	Vector4(-1, 1, -1, 1), // Front, Left
	Vector4(1, 1, -1, 1), // Front, Right
	Vector4(1, 1, 1, 1), // Back, Right
	Vector4(-1, 1, 1, 1), // Back, Left
};

// We're using one normal vector per face, they face the same direction as the side of the cube
static Vector4 faceNormals[] = {
	Vector4(0, -1, 0, 0), // Bottom
	Vector4(0, 1, 0, 0), // Top
	Vector4(0, 0, -1, 0), // Front
	Vector4(0, 0, 1, 0), // Back
	Vector4(-1, 0, 0, 0), // Left
	Vector4(1, 0, 0, 0), // Right
};

static int faces[] = {
	0, 1, 2, 3, // Bottom
	7, 6, 5, 4, // Top
	4, 5, 1, 0, // Front
	3, 2, 6, 7, // Back
	7, 4, 0, 3, // Left
	2, 1, 5, 6, // Right
};
static int faceCount = sizeof(faces) / sizeof(int) / 4;

Model model;

bool CreateScene()
{
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "Error: OpenGL 2.0 required.");
		return false;
	}
	
	if (!model.open("test.model")) {
		return false;
	}
	model.close();
	model.open("test.model");

	glEnable(GL_DEPTH_TEST); // Use Z-Buffer & depth testing
	glEnable(GL_CULL_FACE);

	// Set up projection only once
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 25, 50, 0, 0, 0, 0, 1, 0);

	glEnable(GL_LIGHTING);

	// Enable diffuse lighting
	const GLfloat diffuseColor[] = {0.9, 0.9, 0.9, 1};
	const GLfloat diffuseDirection[] = {0, 0.89, 0.447, 0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_POSITION, diffuseDirection);
	glEnable(GL_LIGHT0);
	
	// Set global ambient light color
	GLfloat globalAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	
	sphere = gluNewQuadric();

	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_SMOOTH);

	return true;
}

void DestroyScene()
{
	model.close();
	gluDeleteQuadric(sphere);
}

void DrawScene(double secondsElapsed)
{
	const double anglePerSecond = 90;
	float angle = (float)(secondsElapsed * anglePerSecond);

	// The following lines enable a "bobbing" of the camera along the Y axis
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, std::sinf(secondsElapsed) * 20, -50, 0, 0, 0, 0, 1, 0);*/

	// We only draw once all messages have been processed
	glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(1, 1, 1);
	
	glPushMatrix();
	
	Quaternion q = Quaternion::fromAxisAndAngle(0, 1, 0, angle * 0.01745329251994329576923690768489);
	Matrix4 m = Matrix4::transformation(Vector4(1, 1, 1, 0), q, Vector4(0, 0, 0, 0));

	glBegin(GL_QUADS);
	for (int i = 0; i < faceCount; i++) {
		glNormal3fv((m * faceNormals[i]).data());

		for (int j = 0; j < 4; ++j) {
			Vector4 transformedVector = m * positions[faces[i * 4 + j]];

			glVertex3fv(transformedVector.data());
		}
	}
	glEnd();
	glPopMatrix();
		
	// Draw earth
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(10, 0, 0);
	gluSphere(sphere, 1, 100, 100);
	glPopMatrix();

	// Draw sun
	//glPushMatrix();
	//gluSphere(sphere, 3, 100, 100);
	//glPopMatrix();

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, model.positionBuffer);
	glVertexPointer(4, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, model.normalBuffer);
	glNormalPointer(GL_FLOAT, sizeof(float) * 4, 0);

	for (int i = 0; i < model.faces; ++i) {
		const FaceGroup &faceGroup = model.faceGroups[i];

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, faceGroup.buffer);
		glDrawElements(GL_TRIANGLES, faceGroup.elementCount, GL_UNSIGNED_SHORT, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPopMatrix();
}

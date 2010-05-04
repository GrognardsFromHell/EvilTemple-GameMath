
#ifndef MODEL_H
#define MODEL_H

#include "../common/common.h"

#include <gl/glew.h>

using namespace GameMath;

struct FaceGroup {
	uint materialId;
	uint elementCount;
	GLuint buffer;
	
	FaceGroup();
	~FaceGroup();
};

class Model {
public:
	Model();
	~Model();

	bool open(const char *filename);
	void close();

	Vector4 *positions;
	Vector4 *normals;
	float *texCoords;
	int vertices;

	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint texcoordBuffer;
	
	int faces;	
	FaceGroup *faceGroups;

	void drawNormals() const;

private:
	// Those two pointers are actually freed, the others just point into these two
	void *vertexData;
	void *faceData;
	
	void loadVertexData();
	void loadFaceData();
};

#endif

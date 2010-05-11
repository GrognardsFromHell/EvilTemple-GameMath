
#ifndef MODEL_H
#define MODEL_H

#include "../common/common.h"

#include <gl/glew.h>

#include "materialstate.h"
#include "renderstate.h"

#include <QtCore/QString>

using namespace GameMath;

struct FaceGroup {
	MaterialState *material;
	uint elementCount;
	GLuint buffer;
	
	FaceGroup();
	~FaceGroup();
};

class Model {
public:
	Model();
	~Model();

	bool open(const char *filename, const RenderStates &renderState);
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

	const QString &error() const;

private:
	// Those two pointers are actually freed, the others just point into these two
	MaterialState *materialState;
	
	void *vertexData;
	void *faceData;
	void *textureData;
	
	void loadVertexData();
	void loadFaceData();

	QString mError;
};

inline const QString &Model::error() const
{
	return mError;
}

#endif

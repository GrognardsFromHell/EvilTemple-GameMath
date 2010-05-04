
#define _CRT_SECURE_NO_WARNINGS

#include "model.h"

#include <stdio.h>

Model::Model()
	: faceGroups(0), positions(0), normals(0), texCoords(0), vertices(0), vertexData(0), faceData(0)
	, positionBuffer(0), normalBuffer(0), texcoordBuffer(0)
{
}

Model::~Model()
{
	delete [] faceGroups;
	if (faceData)
		ALIGNED_FREE(faceData);
	if (vertexData)
		ALIGNED_FREE(vertexData);
}

struct ModelHeader
{
	char magic[4];
	uint version;
	uint checksum;
	uint chunks;
};

struct ChunkHeader
{
	uint type;
	uint flags;
	uint reserved;
	uint size;
};

bool Model::open(const char *filename)
{
	FILE *fp = fopen(filename, "rb");

	if (!fp) {
		return false;
	}

	ModelHeader header;

	if (!fread(&header, sizeof(header), 1, fp)) {
		fprintf(stderr, "Unable to read model file header from %s.\n", filename);
		fclose(fp);
		return false;
	}

	if (header.magic[0] != 'M' || header.magic[1] != 'O' || header.magic[2] != 'D' || header.magic[3] != 'L') {
		fprintf(stderr, "File has invalid magic number: %s.\n", filename);
		fclose(fp);
		return false;
	}
	
	for (uint i = 0; i < header.chunks; ++i) {
		ChunkHeader chunkHeader;

		if (!fread(&chunkHeader, sizeof(ChunkHeader), 1, fp)) {
			fprintf(stderr, "Unable to read chunk %d from file %s.\n", i, filename);
			fclose(fp);
			return false;
		}

		if (chunkHeader.type != 2 && chunkHeader.type != 3) {
			// Skip, unknown chunk
			fprintf(stderr, "WARN: Unknown chunk type %d in model file %s.\n", chunkHeader.type, filename);
			
			fseek(fp, chunkHeader.size, SEEK_CUR);
			continue;
		}

		void *chunkData = ALIGNED_MALLOC(chunkHeader.size);
		
		if (!fread(chunkData, chunkHeader.size, 1, fp)) {
			ALIGNED_FREE(chunkData);
			fprintf(stderr, "Unable to read data of chunk %d in %s.\n", i, filename);
			return false;
		}

		if (chunkHeader.type == 2) {
			vertexData = chunkData;
			loadVertexData();
		} else if (chunkHeader.type == 3) {
			faceData = chunkData;
			loadFaceData();
		} else {
			ALIGNED_FREE(chunkData); // This should never happen
		}
	}

	fclose(fp);

	return true;
}

void Model::close()
{
	if (positionBuffer)
	{
		glDeleteBuffersARB(1, &positionBuffer);
		positionBuffer = 0;
	}

	if (normalBuffer)
	{
		glDeleteBuffersARB(1, &normalBuffer);
		normalBuffer = 0;
	}

	if (texcoordBuffer)
	{
		glDeleteBuffersARB(1, &texcoordBuffer);
		texcoordBuffer = 0;
	}
	
	delete [] faceGroups;
	faceGroups = 0;

	if (faceData) {
		ALIGNED_FREE(faceData);
		faceData = 0;
	}

	if (vertexData) {
		ALIGNED_FREE(vertexData);
		vertexData = 0;
	}
}

struct VertexHeader {
	uint count;
	uint reserved1;
	uint reserved2;
	uint reserved3;
};

void Model::loadVertexData()
{
	VertexHeader *vertexHeader;

	vertexHeader = reinterpret_cast<VertexHeader*>(vertexData);

	vertices = vertexHeader->count;

	positions = reinterpret_cast<Vector4*>(reinterpret_cast<char*>(vertexData) + sizeof(VertexHeader));
	normals = reinterpret_cast<Vector4*>(reinterpret_cast<char*>(vertexData) + sizeof(VertexHeader) + sizeof(Vector4) * vertices);
	texCoords = reinterpret_cast<float*>(reinterpret_cast<char*>(vertexData) + sizeof(VertexHeader) + sizeof(Vector4) * vertices * 2);

	glGenBuffersARB(1, &positionBuffer);
	glGenBuffersARB(1, &normalBuffer);
	glGenBuffersARB(1, &texcoordBuffer);

	for (int i = 0; i < vertices; ++i) {
		positions[i] *= 0.2f;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, positionBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector4) * vertices, positions, GL_STATIC_DRAW_ARB);

	for (int i = 0; i < vertices; ++i) {
		normals[i].normalize();
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector4) * vertices, normals, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, texcoordBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * 2 * vertices, texCoords, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0); // Unbind array buffer
}

struct FacesHeader
{
	uint groups;
	uint reserved1;
	uint reserved2;
	uint reserved3;
};

struct FaceGroupHeader
{
	uint materialId;
	uint elementCount;
	uint elementSize;
	uint reserved;
};

void Model::loadFaceData()
{
	const FacesHeader *header = reinterpret_cast<FacesHeader*>(faceData);
	
	faces = header->groups;
	faceGroups = new FaceGroup[faces];

	char *currentDataPointer = reinterpret_cast<char*>(faceData) + sizeof(FacesHeader);

	for (int i = 0; i < faces; ++i) {
		FaceGroup *faceGroup = faceGroups + i;
		const FaceGroupHeader *groupHeader = reinterpret_cast<FaceGroupHeader*>(currentDataPointer);
		currentDataPointer += sizeof(FaceGroupHeader);

		faceGroup->elementCount = groupHeader->elementCount;
		faceGroup->materialId = groupHeader->materialId;

		uint groupSize = groupHeader->elementCount * groupHeader->elementSize;

		glGenBuffersARB(1, &faceGroup->buffer);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, faceGroup->buffer);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, groupSize, currentDataPointer, GL_STATIC_DRAW_ARB);

		currentDataPointer += groupSize;
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0); // Unbind array buffer
}

void Model::drawNormals() const
{
	glLineWidth(2);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(0, 0, 1);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	for (int i = 0; i < vertices; ++i) {
		Vector4 &vertex = positions[i];
		Vector4 &normal = normals[i];		
		glVertex3fv(vertex.data());
		glVertex3fv((vertex + 5 * normal).data());
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

FaceGroup::FaceGroup() : buffer(0), materialId(0), elementCount(0)
{

}

FaceGroup::~FaceGroup()
{
	if (buffer) {
		//glDeleteBuffersARB(1, &buffer);
	}
}

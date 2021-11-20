#pragma once

#include <glm\glm.hpp>
#include "PerlinNoise.h"
#include "Mesh.h"
#include "Constants.h"

#define MAX_HEIGHT 50
#define NOISE_FUNC_SCALE 100
#define THIRD_VALUE_SEED 0
#define DIRECTION_DIST_SAMPLE 1.0f

class Terrain
{
public:
	Terrain();
	Terrain(unsigned int seed);

	Mesh* getChunkMesh(float x, float y);

	~Terrain();
private:
	PerlinNoise noisefunc;

	float calcSurfaceHeight(float x, float z);
	void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);
};


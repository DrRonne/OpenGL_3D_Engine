#include "Terrain.h"



Terrain::Terrain()
{
	noisefunc = PerlinNoise();
}

Terrain::Terrain(unsigned int seed)
{
	noisefunc = PerlinNoise(seed);
}

Mesh* Terrain::getChunkMesh(float x, float z)
{
	const unsigned int amount_of_points = (CHUNK_SIZE + 1) * (CHUNK_SIZE + 1);
	float points[amount_of_points * 8];
	for (int xi = 0; xi < CHUNK_SIZE + 1; xi++) {
		for (int zi = 0; zi < CHUNK_SIZE + 1; zi++) {
			float xcoord = x + xi * CELL_SIZE;
			float zcoord = z + zi * CELL_SIZE;
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8] = xcoord;
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 1] = calcSurfaceHeight(xcoord, zcoord);
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 2] = zcoord;
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 3] = float(xi);
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 4] = float(zi);
			glm::vec3 v1(DIRECTION_DIST_SAMPLE * 2.0f, calcSurfaceHeight(xcoord + DIRECTION_DIST_SAMPLE, zcoord) - calcSurfaceHeight(xcoord - DIRECTION_DIST_SAMPLE, zcoord), 0.0f);
			glm::vec3 v2(0.0f, calcSurfaceHeight(xcoord, zcoord + DIRECTION_DIST_SAMPLE) - calcSurfaceHeight(xcoord, zcoord - DIRECTION_DIST_SAMPLE), DIRECTION_DIST_SAMPLE * 2.0f);
			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 5] = normal.x;
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 6] = normal.y;
			points[(xi * (CHUNK_SIZE + 1) + zi) * 8 + 7] = normal.z;
		}
	}
	const unsigned int amount_of_indices = CHUNK_SIZE * CHUNK_SIZE * 6;
	unsigned int indices[amount_of_indices];
	for (int xi = 0; xi < CHUNK_SIZE; xi++) {
		for (int yi = 0; yi < CHUNK_SIZE; yi++) {
			indices[(xi * CHUNK_SIZE + yi) * 6] = xi * (CHUNK_SIZE + 1) + yi;
			indices[(xi * CHUNK_SIZE + yi) * 6 + 1] = xi * (CHUNK_SIZE + 1) + yi + 1;
			indices[(xi * CHUNK_SIZE + yi) * 6 + 2] = (xi + 1) * (CHUNK_SIZE + 1) + yi;
			indices[(xi * CHUNK_SIZE + yi) * 6 + 3] = (xi + 1) * (CHUNK_SIZE + 1) + yi;
			indices[(xi * CHUNK_SIZE + yi) * 6 + 4] = xi * (CHUNK_SIZE + 1) + yi + 1;
			indices[(xi * CHUNK_SIZE + yi) * 6 + 5] = (xi + 1) * (CHUNK_SIZE + 1) + yi + 1;
		}
	}

	Mesh* mesh = new Mesh();
	mesh->CreateMesh(points, indices, amount_of_points * 8, amount_of_indices);
	return mesh;
}


Terrain::~Terrain()
{
}

float Terrain::calcSurfaceHeight(float x, float z)
{
	float octave1 = noisefunc.noise(x / NOISE_FUNC_SCALE, z / NOISE_FUNC_SCALE, THIRD_VALUE_SEED) * MAX_HEIGHT - MAX_HEIGHT / 2;
	float octave2 = 0.5 * noisefunc.noise(2 * x / NOISE_FUNC_SCALE, 2 * z / NOISE_FUNC_SCALE, THIRD_VALUE_SEED) * MAX_HEIGHT;
	float octave3 = 0.25 * noisefunc.noise(4 * x / NOISE_FUNC_SCALE, 4 * z / NOISE_FUNC_SCALE, THIRD_VALUE_SEED) * MAX_HEIGHT;
	return octave1 + octave2 + octave3;
}
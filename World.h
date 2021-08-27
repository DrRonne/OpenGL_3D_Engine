#pragma once

#include <vector>

#include "Instance.h"

class World
{
public:
	World();

	std::vector<Instance> GetInstances() { return instances; }

	~World();

private:
	std::vector<Instance> instances;
};


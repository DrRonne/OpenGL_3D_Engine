#pragma once

#include <vector>

#include "Component.h"

class Instance
{
public:
	Instance();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	~Instance();

private:
	std::vector<Component*> components;
};


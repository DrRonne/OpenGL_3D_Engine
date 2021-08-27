#include "Instance.h"



Instance::Instance()
{
}

void Instance::AddComponent(Component* component)
{
	components.push_back(component);
}

void Instance::RemoveComponent(Component* component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == component)
		{
			components.erase(components.begin() + i);
			return;
		}
	}
}


Instance::~Instance()
{
}

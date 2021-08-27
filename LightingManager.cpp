#include "LightingManager.h"



LightingManager::LightingManager()
{
	pointLightCount = 0;
	spotLightCount = 0;
}

void LightingManager::SetMainLight(DirectionalLight light)
{
	mainLight = light;
}

// Returns true if it could be added, otherwise false
bool LightingManager::AddPointLight(PointLight light)
{
	if (pointLightCount >= MAX_POINT_LIGHTS)
		return false;

	pointLights[pointLightCount] = &light;
	pointLightCount++;
	return true;
}

// Returns true if the light was found, false if not
bool LightingManager::RemovePointLight(PointLight light)
{
	// Find the light, remove it from the array and shift the remaining lights
	for (size_t i = 0; i < pointLightCount; i++)
	{
		if (pointLights[i] == &light)
		{
			for (size_t j = i; j < pointLightCount; j++)
			{
				pointLights[j] = pointLights[j + 1];
			}
			return true;
		}
	}
	return false;
}

// Returns true if it could be added, otherwise false
bool LightingManager::AddSpotLight(SpotLight light)
{
	if (spotLightCount >= MAX_SPOT_LIGHTS)
		return false;

	spotLights[spotLightCount] = &light;
	spotLightCount++;
	return true;
}

// Returns true if the light was found, false if not
bool LightingManager::RemoveSpotLight(SpotLight light)
{
	// Find the light, remove it from the array and shift the remaining lights
	for (size_t i = 0; i < spotLightCount; i++)
	{
		if (spotLights[i] == &light)
		{
			for (size_t j = i; j < spotLightCount; j++)
			{
				spotLights[j] = spotLights[j + 1];
			}
			return true;
		}
	}
	return false;
}

LightingManager::~LightingManager()
{
}

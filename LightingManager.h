#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"

class LightingManager
{
public:
	LightingManager();

	DirectionalLight GetMainLight() { return mainLight; }
	void SetMainLight(DirectionalLight light);

	PointLight** GetPointLights() { return pointLights; }
	unsigned int GetPointLightCount() { return pointLightCount; }
	bool AddPointLight(PointLight light);
	bool RemovePointLight(PointLight light);

	SpotLight** GetSpotLights() { return spotLights; }
	unsigned int GetSpotLightCount() { return spotLightCount; }
	bool AddSpotLight(SpotLight light);
	bool RemoveSpotLight(SpotLight light);

	~LightingManager();

private:
	DirectionalLight mainLight;
	PointLight* pointLights[MAX_POINT_LIGHTS];
	SpotLight* spotLights[MAX_SPOT_LIGHTS];
	unsigned int pointLightCount;
	unsigned int spotLightCount;
};


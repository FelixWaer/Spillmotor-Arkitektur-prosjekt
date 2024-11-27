#pragma once

#include "../Engine/GameObject.h"
#include "../GameObject Components/BoxCollision.h"
#include "../Rendering/Model.h"

class BasicSphere;

class Terrain : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

	float get_TerrainHeight(BasicSphere* sphere);
	bool check_IfHitWal(BasicSphere* sphere, glm::vec3& surfaceNormal);
private:
	std::vector<float> GridFriction;

	Model TerrainModel;
	int TerrainXLength = 0;
	int TerrainZLength = 0;
	int Precision = 0;

	void add_Friction();
};


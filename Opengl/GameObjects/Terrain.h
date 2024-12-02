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

	void add_FrictionPoint(const glm::vec3& newPoint);
	void create_Friction();
	float get_TerrainHeight(BasicSphere* sphere);
	bool check_IfHitWal(BasicSphere* sphere, glm::vec3& surfaceNormal);
private:
	std::vector<float> GridFriction;

	Model TerrainModel;
	int TerrainXLength = 0;
	int TerrainZLength = 0;
	int Precision = 0;

	glm::vec3 minFrictionSize = glm::vec3(FLT_MAX);
	glm::vec3 maxFrictionSize = glm::vec3(FLT_MIN);

	void add_Friction();
};


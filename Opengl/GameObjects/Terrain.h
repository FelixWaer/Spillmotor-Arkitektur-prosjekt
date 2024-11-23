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

	void get_TerrainHeight(BasicSphere* sphere);
private:
	Model TerrainModel;
	int TerrainLength = 0;
	int Precision = 0;
};


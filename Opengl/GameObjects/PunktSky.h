#pragma once
#include "../Engine/GameObject.h"
#include "../GameObject Components/BoxCollision.h"
#include "../Rendering/Model.h"

class PunktSky : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

	glm::vec3 get_SlopeAcceleration(glm::vec3& position);
private:
	Model TerrainModel;
	Model TerrainPunktModel;
};


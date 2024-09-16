#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"
#include "../Rendering/Light.h"

class LightCube : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

private:
	Model LightModel;
	Light light;
};


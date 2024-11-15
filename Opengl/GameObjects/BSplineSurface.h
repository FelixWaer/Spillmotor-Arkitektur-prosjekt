#pragma once
#include "../Engine/GameObject.h"
#include "../GameObject Components/BoxCollision.h"
#include "../Rendering/Model.h"

class BSplineSurface : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;
private:
	Model SplineSurface;
};


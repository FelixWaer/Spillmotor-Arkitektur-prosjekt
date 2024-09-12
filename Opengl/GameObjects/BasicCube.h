#pragma once
#include "../Engine/GameObject.h"
#include "../GameObject Components/BoxCollision.h"
#include "../Rendering/Model.h"

class BasicCube : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

private:
	Model BoxModel;
	BoxCollision BoxCollider;
	Event* PhysicsEvent = nullptr;

	void collision_Physics(GameObject* otherGameObject);
};


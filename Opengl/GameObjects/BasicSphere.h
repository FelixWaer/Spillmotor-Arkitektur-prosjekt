#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"
#include "../GameObject Components/SphereCollision.h"

class BasicSphere : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

private:
	Model Cube;
	SphereCollision collider;
	Event* PhysicsEvent = nullptr;
	bool HasCollided = false;

	void collision_Physics(GameObject* otherGameObject);
};


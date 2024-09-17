#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"
#include "../GameObject Components/SphereCollision.h"

class BasicSphere : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;
	float Mass = 1.f;
	bool CanUpdateVelocity = false;
private:
	Model Cube;
	SphereCollision collider;
	Event* PhysicsEvent = nullptr;

	glm::vec3 newVelocity = glm::vec3(0.f);

	void collision_Physics(GameObject* otherGameObject, glm::vec3 hitPosition);
};


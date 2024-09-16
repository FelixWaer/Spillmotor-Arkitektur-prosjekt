#pragma once
#include "../Engine/GameObject.h"
#include "../GameObject Components/BoxCollision.h"
#include "../Rendering/Model.h"

class BasicCube : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;
	glm::vec3 normal = glm::vec3(0.f);
	glm::vec3 normal2 = glm::vec3(0.f);

	Model BoxModel;
private:
	BoxCollision BoxCollider;
	Event* PhysicsEvent = nullptr;
	
	void collision_Physics(GameObject* otherGameObject, glm::vec3 hitPosition);
};


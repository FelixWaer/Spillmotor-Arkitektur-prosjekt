#include "BasicCube.h"

#include <filesystem>

#include "../Engine/EventCallback.h"

void BasicCube::game_Start()
{
	BoxModel.init_Model();
	BoxModel.attach_ToGameObject(this);
	BoxModel.scale_Model(glm::vec3(19.f, 5.f, 1.f));
	BoxModel.set_ModelMesh("Cube");

	BoxCollider.attach_ToGameObject(this);
	BoxCollider.set_BoxWidth(9.5f);
	BoxCollider.set_BoxHeight(2.5f);
	BoxCollider.set_BoxDepth(0.5f);

	PhysicsEvent = make_Event(this, &BasicCube::collision_Physics);
	BoxCollider.attach_Event(PhysicsEvent);
	BoxCollider.enable_BoxVisible(false);

	add_Tag("Wall");

	normal = glm::vec3(0.f, 0.f, 1.f);
	normal2 = glm::vec3(0.f, 1.f, 0.f);
}

void BasicCube::tick(float deltaTime)
{
	GameObject::tick(deltaTime);


}

void BasicCube::collision_Physics(GameObject* otherGameObject, glm::vec3 hitPosition)
{
	
}

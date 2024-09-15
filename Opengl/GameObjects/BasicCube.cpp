#include "BasicCube.h"

#include <filesystem>

#include "../Engine/EventCallback.h"

void BasicCube::game_Start()
{
	BoxModel.init_Model();
	BoxModel.attach_ToGameObject(this);
	BoxModel.scale_Model(glm::vec3(20.f, 10.f, 1.f));
	BoxModel.set_ModelMesh("Cube");

	BoxCollider.attach_ToGameObject(this);
	BoxCollider.set_BoxWidth(20.f);
	BoxCollider.set_BoxHeight(10.f);
	BoxCollider.set_BoxDepth(1.f);

	PhysicsEvent = make_Event(this, &BasicCube::collision_Physics);
	BoxCollider.attach_Event(PhysicsEvent);

	add_Tag("Wall");
}

void BasicCube::tick(float deltaTime)
{
	GameObject::tick(deltaTime);


}

void BasicCube::collision_Physics(GameObject* otherGameObject)
{
	
}

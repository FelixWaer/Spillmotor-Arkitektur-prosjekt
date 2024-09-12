#include "BasicCube.h"

#include "../Engine/EventCallback.h"

void BasicCube::game_Start()
{
	//Initialize the Model -MUST BE DONE
	Cube.init_Model();
	Cube.attach_ToGameObject(this);
	//Set the mesh of the Model

	/*Bind the model to a GameObject, so it will move with the GameObject*/
	//testModel.bind_ToGameObject(this);

	//Set position of the Model
	Cube.set_ModelPosition(glm::vec3(0.f));

	Cube.scale_Model(glm::vec3(1.f));

	collider.attach_ToGameObject(this);
	collider.set_SphereRadius(1.f);
	collider.enable_SphereVisible(true);
	Cube.set_ModelMesh("Ball");
	add_Tag("Ball");

	PhysicsEvent = make_Event(this, &BasicCube::collision_Physics);
	collider.attach_Event(PhysicsEvent);
}

void BasicCube::tick(float deltaTime)
{
	GameObject::tick(deltaTime);

	if (get_GameObjectPosition().x > 20.f || get_GameObjectPosition().x < -20.f)
	{
		get_GameObjectVelocity() *= -1;
		get_GameObjectSpeed() *= 1.5f;
	}
	if (get_GameObjectPosition().y > 20.f || get_GameObjectPosition().y < -20.f)
	{
		get_GameObjectVelocity() *= -1;
		get_GameObjectSpeed() *= 1.5f;
	}
	if (get_GameObjectPosition().z > 20.f || get_GameObjectPosition().z < -20.f)
	{
		get_GameObjectVelocity() *= -1;
		get_GameObjectSpeed() *= 1.5f;
	}
}

void BasicCube::collision_Physics(GameObject* otherGameObject)
{
	if (otherGameObject->has_Tag("Ball"))
	{
		glm::vec3 vector = otherGameObject->get_GameObjectPosition() - get_GameObjectPosition();
		set_GameObjectVelocity(vector * -1.f);
		set_GameObjectSpeed(10.f);

		//std::cout << "other POS: " << otherGameObject->get_GameObjectPosition().x << "this Pos:" << get_GameObjectPosition().x << std::endl;
	}
}

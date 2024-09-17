#include "BasicSphere.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "BasicCube.h"
#include "../Engine/EventCallback.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"0+pp

void BasicSphere::game_Start()
{
	//Initialize the Model -MUST BE DONE
	Cube.init_Model();
	Cube.attach_ToGameObject(this);
	//Set the mesh of the Model

	//Set position of the Model
	//Cube.set_ModelPosition(glm::vec3(0.f));

	Cube.scale_Model(glm::vec3(1.f));

	collider.attach_ToGameObject(this);
	collider.set_SphereRadius(1.f);
	collider.enable_SphereVisible(true);
	Cube.set_ModelMesh("Ball");
	add_Tag("Ball");

	PhysicsEvent = make_Event(this, &BasicSphere::collision_Physics);
	collider.attach_Event(PhysicsEvent);

	GravityEnabled = true;
}

void BasicSphere::tick(float deltaTime)
{
	if (CanUpdateVelocity == true)
	{
		set_GameObjectVelocity(newVelocity);
		CanUpdateVelocity = false;
	}

	GameObject::tick(deltaTime);

}

void BasicSphere::collision_Physics(GameObject* otherGameObject, glm::vec3 hitPosition)
{
	if (otherGameObject->has_Tag("Ball"))
	{
		BasicSphere* otherSphere = static_cast<BasicSphere*>(otherGameObject);

		glm::vec3 directionVector = otherGameObject->get_GameObjectPosition() - get_GameObjectPosition();
		float distance = 2.f - glm::length(directionVector);
		glm::vec3 directionVectorNormal = glm::normalize(directionVector);
		get_GameObjectPosition() += -directionVectorNormal * (distance * 0.5f);

		directionVector = get_GameObjectPosition() - otherGameObject->get_GameObjectPosition();
		float tempMass = (2 * otherSphere->Mass) / (Mass + otherSphere->Mass);

		float dotProduct = glm::dot(get_GameObjectVelocity() - otherSphere->get_GameObjectVelocity(), directionVector);

		float magnitude = static_cast<float>(sqrt(pow(directionVector.x, 2) + pow(directionVector.y, 2) + pow(directionVector.z, 2)));
		dotProduct /= magnitude;

		glm::vec3 newDirection = tempMass * dotProduct * directionVector;

		newVelocity = get_GameObjectVelocity() - newDirection;

		set_GameObjectSpeed(10.f);

		otherSphere->CanUpdateVelocity = true;
	}
	if (otherGameObject->has_Tag("Wall"))
	{
		glm::vec3 hitPositionNormal = glm::normalize(hitPosition-get_GameObjectPosition());
		float dotProduct = glm::dot(get_GameObjectVelocity(), hitPositionNormal);
		glm::vec3 test = hitPositionNormal * dotProduct;

		test *= -2;
		test += get_GameObjectVelocity();
		set_GameObjectVelocity(test);
	}
}

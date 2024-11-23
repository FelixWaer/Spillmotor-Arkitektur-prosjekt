#include "BasicSphere.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "BasicCube.h"
#include "../Engine/EngineManager.h"
#include "../Engine/EventCallback.h"
#include "../Engine/Input.h"
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
	GravityEvent = make_Event(this, &BasicSphere::turn_OnGravityButton);

	collider.attach_Event(PhysicsEvent);
	Input::bind_EventToKey(GravityEvent, Key::E, KeyPress::OnPress);

	/*Mesh* test = EngineManager::get()->get_Mesh("TriangulatedMesh");

	glm::ivec2 coords(0);
	coords.x = get_GameObjectPosition().x * 100;
	coords.y = get_GameObjectPosition().z * 100;

	coords >>= 5;
	int index = coords.x + (coords.y * 1490);*/

	//get_GameObjectPosition().y = test->Vertices[index].Position.y + 0.5f;

	std::cout << "xpos" << get_GameObjectPosition().x << std::endl;

	//FLXMath::calculate_PointOnTriangle(get_GameObjectPosition(), test->Vertices[index].Position,
	//	test->Vertices[index + 1355], test->Vertices[index + 1], get_GameObjectPosition())
}

void BasicSphere::tick(float deltaTime)
{
	if (CanUpdateVelocity == true)
	{
		set_GameObjectVelocity(newVelocity);
		CanUpdateVelocity = false;
	}
	GameObject::tick(deltaTime);

	if (TriangulatedTerrain != nullptr)
	{
		TriangulatedTerrain->get_TerrainHeight(this);
	}

}

void BasicSphere::set_Terrain(Terrain* terrain)
{
	TriangulatedTerrain = terrain;
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
		float speed = glm::length(otherSphere->get_GameObjectVelocity()) + glm::length(get_GameObjectVelocity());
		float tempMass = (2 * otherSphere->Mass) / (Mass + otherSphere->Mass);

		float dotProduct = glm::dot(get_GameObjectVelocity() - otherSphere->get_GameObjectVelocity(), directionVector);

		float magnitude = static_cast<float>(sqrt(pow(directionVector.x, 2) + pow(directionVector.y, 2) + pow(directionVector.z, 2)));
		dotProduct /= magnitude;

		glm::vec3 newDirection = tempMass * dotProduct * directionVector;

		newVelocity = glm::normalize(get_GameObjectVelocity() - newDirection) * (speed*0.5f);

		otherSphere->CanUpdateVelocity = true;
	}
	if (otherGameObject->has_Tag("Wall"))
	{
		glm::vec3 hitPositionNormal = glm::normalize(hitPosition-get_GameObjectPosition());
		float length = glm::length(hitPosition - get_GameObjectPosition());
		get_GameObjectPosition() += -hitPositionNormal * (1.f - length);


		float speed = glm::length(get_GameObjectVelocity());
		float dotProduct = glm::dot(get_GameObjectVelocity(), hitPositionNormal);
		glm::vec3 test = hitPositionNormal * dotProduct;

		test *= -2;
		test += get_GameObjectVelocity();
		set_GameObjectVelocity(glm::normalize(test)*speed);
	}
}

void BasicSphere::turn_OnGravityButton()
{
	enable_Gravity(true);
}

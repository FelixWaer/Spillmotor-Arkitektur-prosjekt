#include "BasicSphere.h"

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
}

void BasicSphere::tick(float deltaTime)
{
	GameObject::tick(deltaTime);

}

void BasicSphere::collision_Physics(GameObject* otherGameObject)
{
	//if (HasCollided == true)
	//{
	//	return;
	//}

	if (otherGameObject->has_Tag("Ball"))
	{
		glm::vec3 vector = otherGameObject->get_GameObjectPosition() - get_GameObjectPosition();
		set_GameObjectVelocity(vector * -1.f);
		set_GameObjectSpeed(10.f);

		//std::cout << "other POS: " << otherGameObject->get_GameObjectPosition().x << "this Pos:" << get_GameObjectPosition().x << std::endl;
	}
	if (otherGameObject->has_Tag("Wall"))
	{
		glm::vec3 direction = get_GameObjectVelocity();
	/*	float testing = FLXMath::calculate_AngleBetweenVectors(static_cast<BasicCube*>(otherGameObject)->normal, direction);
		float testing2 = FLXMath::calculate_AngleBetweenVectors(static_cast<BasicCube*>(otherGameObject)->normal2, direction);*/

		glm::vec3 normalVector;

		normalVector = static_cast<BasicCube*>(otherGameObject)->normal;

		float dotProduct = glm::dot(get_GameObjectVelocity(), normalVector);
		glm::vec3 test = normalVector * dotProduct;

		test *= -2;
		test += get_GameObjectVelocity();
		set_GameObjectVelocity(test);


		//glm::vec3 pos = glm::vec3(0.f, 0.f, -1.f);
		//get_GameObjectPosition() += pos * collider.get_SphereRadius();


		//float testDot = glm::dot(get_GameObjectPosition(), glm::vec3(0.f, 0.f, 1.f));
		//float testDot2 = glm::dot(get_GameObjectPosition(), glm::vec3(1.f, 0.f, 0.f));
		//std::cout << "same direction: " << testing << " Other Direction: " << testing2 << std::endl;
		//glm::vec3 pos2 = glm::vec3(20.f, 0.f, 0.f);
		//float angle = FLXMath::calculate_AngleBetweenVectors(pos, direction);
		//float angle2 = FLXMath::calculate_AngleBetweenVectors(pos2, direction);
		//std::cout << "angle between wall 1: " << angle << std::endl;
		//std::cout << "angle between wall 2: " << angle2 << std::endl;

		HasCollided = true;
	}
}

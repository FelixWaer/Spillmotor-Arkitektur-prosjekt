#include "BasicSphere.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "BasicCube.h"
#include "../Engine/EngineManager.h"
#include "../Engine/EventCallback.h"
#include "../Engine/Input.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"

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

	get_Mass() = 1.f;

	BSpline.init_GameObject();
}

void BasicSphere::tick(float deltaTime)
{
	if (CanUpdateVelocity == true)
	{
		set_GameObjectVelocity(newVelocity);
		CanUpdateVelocity = false;
	}
	GameObject::tick(deltaTime);

	BSpline.hide_BSpline(BasicSphere::HidingBSpline);

	if (TriangulatedTerrain != nullptr)
	{
		float height = TriangulatedTerrain->get_TerrainHeight(this);

		if (get_GameObjectPosition().y < height)
		{
			get_GameObjectPosition().y = height;
		}
		else
		{
			set_Acceleration(glm::vec3(0.f, -9.81f, 0.f));
		}

		glm::vec3 surfaceNormal(0.f);
		if (TriangulatedTerrain->check_IfHitWal(this, surfaceNormal) == true)
		{
			bounce_BallOfGround(surfaceNormal);
			//std::cout << "you are hitting the wall" << std::endl;
		}
	}

	BSplineTimer += deltaTime;
	if (BSplineTimer >= 1.f)
	{
		BSpline.add_ControlPoint(get_GameObjectPosition());
		BSpline.generate_BSpline();

		BSplineTimer = 0.f;
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

void BasicSphere::bounce_BallOfGround(glm::vec3& surfaceNormal)
{
	//std::cout << "Before speed" << glm::length(get_GameObjectVelocity()) << std::endl;

	float speed = glm::length(get_GameObjectVelocity()) * 0.5f;
	float dotProduct = glm::dot(get_GameObjectVelocity(), surfaceNormal);
	glm::vec3 test = surfaceNormal * dotProduct;

	test *= -2;
	test += glm::normalize(get_GameObjectVelocity());
	set_GameObjectVelocity(glm::normalize(test) * speed);

	//std::cout << "After speed" << speed << std::endl;
}

void BasicSphere::turn_OnGravityButton()
{
	enable_Gravity(true);
}

#include "SceneManager.h"

#include "glm/glm.hpp"
#include <glm/ext/matrix_clip_space.hpp>

void SceneManager::begin_Scene()
{
	ActiveEngineCamera.init_GameObject();
	//SphereObject.init_GameObject();
	//SphereObject.set_GameObjectPosition(glm::vec3(0.f, 0.f, 1.f));
	//CubeObject.set_GameObjectSpeed(10.f);

	//SphereObject2.init_GameObject();
	//SphereObject2.set_GameObjectPosition(glm::vec3(10.f, 0.f, 0.f));
	//SphereObject2.set_GameObjectVelocity(glm::vec3(-1.f, 0.f, 0.f));
	////CubeObject2.set_GameObjectSpeed(10.f);

	//SphereObject3.init_GameObject();
	//SphereObject3.set_GameObjectPosition(glm::vec3(-20.f, 0.f, 0.f));
	//SphereObject3.set_GameObjectVelocity(glm::vec3(1.f, 0.f, 0.f));
	//CubeObject3.set_GameObjectSpeed(10.f);

	Wall1.init_GameObject();
	Wall1.set_GameObjectPosition(glm::vec3(0.f, 0.f, 10.f));
	Wall2.init_GameObject();
	Wall2.set_GameObjectPosition(glm::vec3(0.f, 0.f, -10.f));
	Wall3.init_GameObject();
	Wall3.set_GameObjectPosition(glm::vec3(10.f, 0.f, 0.f));
	Wall3.BoxModel.rotate_Model(glm::vec3(0.f, 90.f, 0.f));
	Wall4.init_GameObject();
	Wall4.set_GameObjectPosition(glm::vec3(-10.f, 0.f, 0.f));
	Wall4.BoxModel.rotate_Model(glm::vec3(0.f, 90.f, 0.f));

	SceneLight.init_GameObject();
	SceneLight.set_GameObjectPosition(glm::vec3(0.f, 20.f, 0.f));

	while (GameObjectsToBeAdded.empty() == false)
	{
		GameObject* addedGameObject = GameObjectsToBeAdded.front();
		GameObjectsToBeAdded.pop(),

		addedGameObject->game_Start();
		SceneGameObjects.emplace_back(addedGameObject);
	}
}



void SceneManager::tick_Scene(float deltaTime)
{
	while (GameObjectsToBeAdded.empty() == false)
	{
		GameObject* addedGameObject = GameObjectsToBeAdded.front();
		GameObjectsToBeAdded.pop(),

		addedGameObject->game_Start();
		SceneGameObjects.emplace_back(addedGameObject);
	}

	check_Collision();
	tick_GameObjects(deltaTime);
}

void SceneManager::set_SceneCamera(Camera* newSceneCamera)
{
	SceneCamera = newSceneCamera;
}

Camera* SceneManager::get_SceneCamera()
{
	return SceneCamera;
}

std::vector<Model*>& SceneManager::get_SceneModels()
{
	return SceneModels;
}

std::vector<Light*>& SceneManager::get_SceneLights()
{
	return SceneLights;
}

void SceneManager::add_GameObjectToScene(GameObject* gObject)
{
	GameObjectsToBeAdded.emplace(gObject);
}

void SceneManager::add_BoxColliderToScene(BoxCollision* boxCollider)
{
	SceneBoxColliders.emplace_back(boxCollider);
}

void SceneManager::add_SphereColliderToScene(SphereCollision* sphereCollider)
{
	SceneSphereColliders.emplace_back(sphereCollider);
}

void SceneManager::add_ModelToScene(Model* model)
{
	SceneModels.emplace_back(model);
}

void SceneManager::add_LightToScene(Light* light)
{
	SceneLights.emplace_back(light);
}

void SceneManager::turnOff_DebugMode(bool turnOff)
{
	for (SphereCollision* sphere : SceneSphereColliders)
	{
		sphere->enable_SphereVisible(turnOff);
	}
	for (BoxCollision* sphere : SceneBoxColliders)
	{
		sphere->enable_BoxVisible(turnOff);
	}
}

void SceneManager::tick_GameObjects(float deltaTime)
{
	for (GameObject* gObject : SceneGameObjects)
	{
		gObject->tick(deltaTime);
	}
}

void SceneManager::check_Collision()
{
	if (SceneBoxColliders.size() > 1)
	{
		for (size_t i = 0; i < SceneBoxColliders.size(); i++)
		{
			for (size_t j = i + 1; j < SceneBoxColliders.size(); j++)
			{
				if (calculate_BoxCollision(SceneBoxColliders[i]->get_BoxPosition(), SceneBoxColliders[j]->get_BoxPosition(),
					SceneBoxColliders[i]->BoxHeight, SceneBoxColliders[i]->BoxWidth, SceneBoxColliders[i]->BoxDepth,
					SceneBoxColliders[j]->BoxHeight, SceneBoxColliders[j]->BoxWidth, SceneBoxColliders[j]->BoxDepth))
				{
					SceneBoxColliders[i]->call_CollisionEvent(SceneBoxColliders[j], glm::vec3(0.f));
					SceneBoxColliders[j]->call_CollisionEvent(SceneBoxColliders[i], glm::vec3(0.f));
				}
			}
		}
	}

	if (SceneSphereColliders.size() > 1)
	{
		for (size_t i = 0; i < SceneSphereColliders.size(); i++)
		{
			for (size_t j = i + 1; j < SceneSphereColliders.size(); j++)
			{
				if (calculate_SphereCollision(SceneSphereColliders[i]->get_SpherePosition(), SceneSphereColliders[j]->get_SpherePosition(),
					SceneSphereColliders[i]->get_SphereRadius(), SceneSphereColliders[j]->get_SphereRadius()))
				{
					SceneSphereColliders[i]->call_CollisionEvent(SceneSphereColliders[j], glm::vec3(0.f));
					SceneSphereColliders[j]->call_CollisionEvent(SceneSphereColliders[i], glm::vec3(0.f));
				}
			}
		}
	}

	if (SceneSphereColliders.empty() == false && SceneBoxColliders.empty() == false)
	{
		glm::vec3 hitPosition;
		for (size_t i = 0; i < SceneSphereColliders.size(); i++)
		{
			for (size_t j = 0; j < SceneBoxColliders.size(); j++)
			{
				if (calculate_BoxSphereCollision(*SceneBoxColliders[j], *SceneSphereColliders[i], hitPosition) == true)
				{
					SceneBoxColliders[j]->call_CollisionEvent(SceneSphereColliders[i], hitPosition);
					SceneSphereColliders[i]->call_CollisionEvent(SceneBoxColliders[j], hitPosition);
				}
			}
		}
	}
}

bool SceneManager::calculate_BoxCollision(glm::vec3 boxPos_1, glm::vec3 boxPos_2, float boxHeight_1, float boxWidth_1, float boxDepth_1, float boxHeight_2, float boxWidth_2, float boxDepth_2)
{
	return (boxPos_1.x - boxWidth_1 / 2) <= (boxPos_2.x + boxWidth_2 / 2) &&
		(boxPos_1.x + boxWidth_1 / 2) >= (boxPos_2.x - boxWidth_2 / 2) &&
		(boxPos_1.y - boxHeight_1 / 2) <= (boxPos_2.y + boxHeight_2 / 2) &&
		(boxPos_1.y + boxHeight_1 / 2) >= (boxPos_2.y - boxHeight_2 / 2) &&
		(boxPos_1.z - boxDepth_1 / 2) <= (boxPos_2.z + boxDepth_2 / 2) &&
		(boxPos_1.z + boxDepth_1 / 2) >= (boxPos_2.z - boxDepth_2 / 2);
}

bool SceneManager::calculate_SphereCollision(glm::vec3 spherePos_1, glm::vec3 spherePos_2, float sphereRadius_1, float sphereRadius_2)
{
	float distance = pow((spherePos_1.x - spherePos_2.x), 2) + pow((spherePos_1.y - spherePos_2.y), 2) + pow((spherePos_1.z - spherePos_2.z), 2);
	if (sqrt(distance) < sphereRadius_1 + sphereRadius_2)
	{

		return true;
	}
	else
	{
		return false;
	}
}

bool SceneManager::calculate_BoxSphereCollision(const BoxCollision& boxCollider, const SphereCollision& sphereCollider, glm::vec3& hitPosition)
{
	glm::vec3 min = boxCollider.get_BoxPosition();
	min.x -= boxCollider.BoxWidth;
	min.y -= boxCollider.BoxHeight;
	min.z -= boxCollider.BoxDepth;

	glm::vec3 max = boxCollider.get_BoxPosition();
	max.x += boxCollider.BoxWidth;
	max.y += boxCollider.BoxHeight;
	max.z += boxCollider.BoxDepth;

	glm::vec3 closesPoint(0.f);

	closesPoint = glm::clamp(sphereCollider.get_SpherePosition(), min, max);

	float distance = glm::distance(sphereCollider.get_SpherePosition(), closesPoint);

	if (distance <= sphereCollider.get_SphereRadius())
	{
		hitPosition = closesPoint;
		return true;
	}

	return false;
}

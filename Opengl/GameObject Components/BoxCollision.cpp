#include "BoxCollision.h"

#include "../Engine/EngineManager.h"
#include "../Engine/EventCallback.h"
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"

void BoxCollision::attach_ToGameObject(GameObject* GO)
{
	AttachedGameObject = GO;
	BoxPosition = GO->get_GameObjectPositionPtr();
	EngineManager::get()->get_ActiveScene()->add_BoxColliderToScene(this);
}

void BoxCollision::attach_Event(Event* eventPtr)
{
	CollisionEvent = eventPtr;
}

void BoxCollision::enable_Collider(bool isColliding)
{

}

void BoxCollision::enable_BoxVisible(bool isVisible)
{
	if (isVisible == true && BoxModel == nullptr)
	{
		BoxModel = new Model;
		BoxModel->init_Model();
		//BoxModel->draw_ModelAsLines(true);
		BoxModel->attach_ToPosition(BoxPosition);
		BoxModel->scale_Model(glm::vec3(BoxWidth, BoxHeight, BoxDepth));
		BoxModel->set_ModelMesh("Cube");
		return;
	}
	if (isVisible == false && BoxModel != nullptr)
	{
		BoxModel->hide_Model(true);
	}
}

glm::vec3 BoxCollision::get_BoxPosition() const
{
	return *BoxPosition;
}

GameObject* BoxCollision::get_AttachedGameObject()
{
	return AttachedGameObject;
}

void BoxCollision::set_BoxHeight(float height)
{
	BoxHeight = height;
}

void BoxCollision::set_BoxWidth(float width)
{
	BoxWidth = width;
}

void BoxCollision::set_BoxDepth(float depth)
{
	BoxDepth = depth;
}

void BoxCollision::call_CollisionEvent(BoxCollision* otherBoxCollider, glm::vec3 hitPosition) const
{
	if (CollisionEvent != nullptr)
	{
		CollisionEvent->Collision_Event(otherBoxCollider->get_AttachedGameObject(), hitPosition);
	}
}

void BoxCollision::call_CollisionEvent(SphereCollision* otherSphereCollider, glm::vec3 hitPosition) const
{
	if (CollisionEvent != nullptr)
	{
		CollisionEvent->Collision_Event(otherSphereCollider->get_AttachedGameObject(), hitPosition);
	}
}

void BoxCollision::calculate_BoundingBox(Model* model)
{
	Mesh* meshPtr = EngineManager::get()->get_Mesh(model->get_ModelMeshName());

	//float minX = FLT_MAX;
	//float minY = FLT_MAX;
	//float minZ = FLT_MAX;
	//float maxX = -FLT_MAX;
	//float maxY = -FLT_MAX;
	//float maxZ = -FLT_MAX;

	MinVertex = glm::vec3(FLT_MAX);
	MaxVertex = glm::vec3(-FLT_MAX);

	glm::vec3 test;

	for (const Vertex& vertex : meshPtr->Vertices)
	{
		test = glm::vec3(model->get_ModelMatrix() * glm::vec4(vertex.Position, 1.0f));

		//minX = glm::min(minX, test.x);
		//minY = glm::min(minY, test.y);
		//minZ = glm::min(minZ, test.z);
		//maxX = glm::max(maxX, test.x);
		//maxY = glm::max(maxY, test.y);
		//maxZ = glm::max(maxZ, test.z);

		MinVertex = glm::min(MinVertex, test);
		MaxVertex = glm::max(MaxVertex, test);
	}

	//std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	//std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;
	std::cout << "Vector" << std::endl;
	std::cout << "minX: " << MinVertex.x << " minY: " << MinVertex.y << " minZ: " << MinVertex.z << std::endl;
	std::cout << "maxX: " << MaxVertex.x << " maxY: " << MaxVertex.y << " maxZ: " << MaxVertex.z << std::endl;
	std::cout << "----------------------------" << std::endl;
}


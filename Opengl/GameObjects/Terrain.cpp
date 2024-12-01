#include "Terrain.h"

#include "../Engine/EngineManager.h"
#include "../Rendering/ModelCreation.h"
#include "../Rendering/RenderManager.h"
#include "../GameObjects/BasicSphere.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"
#include "glad/glad.h"

void Terrain::game_Start()
{
	set_GameObjectPosition(glm::vec3(0.f));

	glm::vec3 temp = FLXModel::triangulate_Terrain(*EngineManager::get()->get_Mesh("Leira"), *EngineManager::get()->get_Mesh("TriangulatedMesh"));

	Precision = temp.x;
	TerrainXLength = temp.y;
	TerrainZLength = temp.z;

	std::cout << TerrainXLength << std::endl;

	GridFriction.resize((TerrainXLength - 1) * (TerrainZLength - 1), 0);
	add_Friction();

	TerrainModel.init_Model();
	TerrainModel.attach_ToGameObject(this);
	TerrainModel.scale_Model(glm::vec3(1.f));
	TerrainModel.rotate_Model(glm::vec3(0.f, 0.f, 0.f));
	TerrainModel.set_ModelPosition(glm::vec3(0.f));
	TerrainModel.set_ModelMesh("TriangulatedMesh");
	TerrainModel.hide_Model(false);

	std::cout << "Terrain created" << std::endl;
}

void Terrain::tick(float deltaTime)
{

}

float Terrain::get_TerrainHeight(BasicSphere* sphere)
{
	std::vector<Vertex>& verticesRef = EngineManager::get()->get_Mesh("TriangulatedMesh")->Vertices;
	float height = sphere->get_GameObjectPosition().y;
	glm::ivec2 coords(0);
	coords.x = static_cast<int>(sphere->get_GameObjectPosition().x * 100.f);
	coords.y = static_cast<int>(sphere->get_GameObjectPosition().z * 100.f);

	coords >>= Precision;
	int index1 = coords.x + (coords.y * TerrainXLength);
	int index2 = index1 + TerrainXLength;

	if (index1 < 0 || index2 >= verticesRef.size())
	{
		return height;
	}

	int index3 = index1 + TerrainXLength + 1;
	int index4 = index1 + 1;

	float frictionValue = GridFriction[index1];

	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index2].Position, verticesRef[index3].Position, sphere->get_Acceleration(), height);
	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index3].Position, verticesRef[index4].Position, sphere->get_Acceleration(), height);

	glm::vec3 n = glm::cross(verticesRef[index2].Position - verticesRef[index1].Position,
		verticesRef[index3].Position - verticesRef[index1].Position);
	n = glm::normalize(n);

	float friction = FLXMath::calculate_FrictionForce(n, frictionValue);

	glm::vec3 frictionForce = glm::normalize(sphere->get_GameObjectVelocity());
	frictionForce *= -1.f;
	frictionForce *= friction;

	//std::cout << "friction force: " << frictionForce.x << " " << frictionForce.y << " " << frictionForce.z << std::endl;

	sphere->get_Acceleration() += frictionForce;

	return height;
}

bool Terrain::check_IfHitWal(BasicSphere* sphere, glm::vec3& surfaceNormal)
{
	std::vector<Vertex>& verticesRef = EngineManager::get()->get_Mesh("TriangulatedMesh")->Vertices;
	float height = 0.f;
	glm::ivec2 coords(0);
	glm::vec3 point = sphere->get_GameObjectPosition();
	point += glm::normalize(sphere->get_GameObjectVelocity()) * glm::vec3(1.f, 0.f, 1.f);

	//std::cout << point.x << " " << point.y << " " << point.z << std::endl;
	
	coords.x = static_cast<int>(point.x * 100.f);
	coords.y = static_cast<int>(point.z * 100.f);

	coords >>= Precision;
	int index1 = coords.x + (coords.y * TerrainXLength);

	if (index1 < 0 || index1 >= verticesRef.size())
	{
		return false;
	}

	int index2 = index1 + TerrainXLength;
	int index3 = index1 + TerrainXLength + 1;
	int index4 = index1 + 1;

	glm::vec3 surfaceNormalTemp;
	glm::vec3 temp(0.f);

	if (FLXMath::calculate_PointOnTriangle(point, verticesRef[index1].Position,
		verticesRef[index2].Position, verticesRef[index3].Position, temp, height) == true)
	{
		
		surfaceNormalTemp = glm::cross(verticesRef[index2].Position - verticesRef[index1].Position,
			verticesRef[index3].Position - verticesRef[index1].Position);
	}
	else
	{
		surfaceNormalTemp = glm::cross(verticesRef[index3].Position - verticesRef[index1].Position,
			verticesRef[index4].Position - verticesRef[index1].Position);
	}

	float surfaceAngle = FLXMath::calculate_SurfaceAngle(surfaceNormalTemp);
	surfaceNormal = surfaceNormalTemp;
	if (surfaceAngle >= 60.f && point.y <= height)
	{
		return true;
	}

	return false;
}

void Terrain::add_Friction()
{
	std::vector<Vertex>& verticesRef = EngineManager::get()->get_Mesh("TriangulatedMesh")->Vertices;

	for (int x = 250; x < 500; x++)
	{
		for (int z = 250; z < 600; z++)
		{
			int index1 = x + (z * TerrainXLength);
			int index2 = index1 + TerrainXLength;
			int index3 = index1 + TerrainXLength + 1;
			int index4 = index1 + 1;
			GridFriction[index1] = 0.3f;

			verticesRef[index1].Color = glm::vec3(1.f, 0.f, 0.f);
			verticesRef[index2].Color = glm::vec3(1.f, 0.f, 0.f);
			verticesRef[index3].Color = glm::vec3(1.f, 0.f, 0.f);
			verticesRef[index4].Color = glm::vec3(1.f, 0.f, 0.f);
		}
	}

	EngineManager::get()->get_Mesh("TriangulatedMesh")->rebind_Buffer(GL_STATIC_DRAW);
}

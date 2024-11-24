#include "Terrain.h"

#include "../Engine/EngineManager.h"
#include "../Rendering/ModelCreation.h"
#include "../Rendering/RenderManager.h"
#include "../GameObjects/BasicSphere.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"

void Terrain::game_Start()
{
	set_GameObjectPosition(glm::vec3(0.f));

	glm::vec2 temp = FLXModel::triangulate_Terrain(*EngineManager::get()->get_Mesh("CroppedCloud"), *EngineManager::get()->get_Mesh("TriangulatedMesh"));

	TerrainLength = temp.x;
	Precision = temp.y;

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
	int index1 = coords.x + (coords.y * TerrainLength);

	if (index1 < 0)
	{
		return height;
	}

	int index2 = index1 + TerrainLength;
	int index3 = index1 + TerrainLength + 1;
	int index4 = index1 + 1;

	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index2].Position, verticesRef[index3].Position, sphere->get_Acceleration(), height);
	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index3].Position, verticesRef[index4].Position, sphere->get_Acceleration(), height);

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
	int index1 = coords.x + (coords.y * TerrainLength);

	if (index1 < 0 || index1 >= verticesRef.size())
	{
		return false;
	}

	int index2 = index1 + TerrainLength;
	int index3 = index1 + TerrainLength + 1;
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
	if (surfaceAngle >= 70.f && point.y <= height)
	{
		return true;
	}

	return false;
}

#include "Terrain.h"

#include "../Engine/EngineManager.h"
#include "../Rendering/ModelCreation.h"
#include "../Rendering/RenderManager.h"
#include "../GameObjects/BasicSphere.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"

void Terrain::game_Start()
{
	set_GameObjectPosition(glm::vec3(0.f));

	glm::vec2 temp = FLXModel::triangulate_Terrain(*EngineManager::get()->get_Mesh("Leira"), *EngineManager::get()->get_Mesh("TriangulatedMesh"));

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

void Terrain::get_TerrainHeight(BasicSphere* sphere)
{
	std::vector<Vertex>& verticesRef = EngineManager::get()->get_Mesh("TriangulatedMesh")->Vertices;
	glm::ivec2 coords(0);
	coords.x = static_cast<int>(sphere->get_GameObjectPosition().x * 100.f);
	coords.y = static_cast<int>(sphere->get_GameObjectPosition().z * 100.f);

	coords >>= Precision;
	int index1 = coords.x + (coords.y * TerrainLength);

	int index2 = index1 + TerrainLength;
	int index3 = index1 + TerrainLength + 1;
	int index4 = index1 + 1;

	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index2].Position, verticesRef[index3].Position, sphere->get_Acceleration());
	FLXMath::calculate_PointOnTriangle(sphere->get_GameObjectPosition(), verticesRef[index1].Position,
		verticesRef[index3].Position, verticesRef[index4].Position, sphere->get_Acceleration());
}

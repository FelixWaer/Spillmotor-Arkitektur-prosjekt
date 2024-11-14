#include "PunktSky.h"

void PunktSky::game_Start()
{
	TerrainModel.init_Model();
	TerrainModel.attach_ToGameObject(this);
	TerrainModel.scale_Model(glm::vec3(0.5f));
	TerrainModel.rotate_Model(glm::vec3(-90.f, 0.f, 0.f));
	//TerrainModel.set_ModelMesh("CroppedCloud");
	TerrainModel.set_ModelMesh("TriangulatedMesh");
	TerrainModel.hide_Model(false);
}

void PunktSky::tick(float deltaTime)
{
}

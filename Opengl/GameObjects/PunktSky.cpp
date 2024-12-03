#include "PunktSky.h"

void PunktSky::game_Start()
{
	//TerrainModel.init_Model();
	//TerrainModel.attach_ToGameObject(this);
	//TerrainModel.scale_Model(glm::vec3(1.f));
	//TerrainModel.rotate_Model(glm::vec3(0.f, 0.f, 0.f));
	//TerrainModel.set_ModelPosition(glm::vec3(0.f));
	//TerrainModel.set_ModelMesh("TriangulatedMesh");
	//TerrainModel.hide_Model(false);
	set_GameObjectPosition(glm::vec3(0.f));

	TerrainPunktModel.init_Model();
	TerrainPunktModel.attach_ToGameObject(this);
	TerrainPunktModel.scale_Model(glm::vec3(1.f));
	TerrainPunktModel.rotate_Model(glm::vec3(0.f, 0.f, 0.f));
	TerrainPunktModel.set_ModelPosition(glm::vec3(0.f, 0.f, 0.f));
	TerrainPunktModel.set_ModelMesh("Ski");
	//TerrainPunktModel.hide_Model(false);
}

void PunktSky::tick(float deltaTime)
{
}

void PunktSky::hide_PunktSky(bool hidingModel)
{
	TerrainPunktModel.hide_Model(hidingModel);
}

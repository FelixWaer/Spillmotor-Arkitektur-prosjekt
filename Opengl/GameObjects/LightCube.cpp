#include "LightCube.h"

void LightCube::game_Start()
{
	LightModel.init_Model();
	LightModel.set_ModelMesh("ReverseCube");
	LightModel.attach_ToGameObject(this);

	light.init_Light();
}

void LightCube::tick(float deltaTime)
{
	light.set_LightPosition(get_GameObjectPosition());
}

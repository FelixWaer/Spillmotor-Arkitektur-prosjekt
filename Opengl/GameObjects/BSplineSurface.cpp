#include "BSplineSurface.h"

void BSplineSurface::game_Start()
{
	SplineSurface.init_Model();
	SplineSurface.attach_ToGameObject(this);
	SplineSurface.scale_Model(glm::vec3(20.0f));
	SplineSurface.rotate_Model(glm::vec3(-90.f, 0.f, 0.f));
	SplineSurface.set_ModelMesh("BSplineSurface");
	SplineSurface.hide_Model(false);
}

void BSplineSurface::tick(float deltaTime)
{
}

#pragma once
#include <queue>

#include "../Engine/GameObject.h"
#include "../Rendering/Camera.h"
#include "../Engine/EventCallback.h"
#include "../GameObject Components/SphereCollision.h"

class BasicSphere;
class Terrain;
class PunktSky;
class LightCube;

class EngineCamera : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;
	void set_Terrain(Terrain* terrain);

private:
	void input_WFunction();
	void input_AFunction();
	void input_SFunction();
	void input_DFunction();
	void input_OneFunction();
	void input_TwoFunction();
	void input_ThreeFunction();
	void input_FourFunction();
	void input_FiveFunction();
	void input_SixFunction();
	void input_LMouseFunction();
	void input_RMouseFunction();
	void input_ESCFunction();
	void collision_Function(GameObject* otherGameObject, glm::vec3 hitPosition);

	Event* W_InputEvent = nullptr;
	Event* A_InputEvent = nullptr;
	Event* S_InputEvent = nullptr;
	Event* D_InputEvent = nullptr;
	Event* One_InputEvent = nullptr;
	Event* Two_InputEvent = nullptr;
	Event* Three_InputEvent = nullptr;
	Event* Four_InputEvent = nullptr;
	Event* Five_InputEvent = nullptr;
	Event* Six_InputEvent = nullptr;
	Event* LM_InputEvent = nullptr;
	Event* RM_InputEvent = nullptr;
	Event* ESC_InputEvent = nullptr;
	Event* CollisionEvent = nullptr;

	Camera ActiveCamera;
	Terrain* TriangulatedTerrain = nullptr;
	PunktSky* PunktSkyModel = nullptr;
	LightCube* Light = nullptr;
	//SphereCollision SphereCollider;
	std::vector<BasicSphere*> Balls;
	int MakeFriction = 0;
	bool collisionCheck = false;
	bool hidingPunktSky = false;
};


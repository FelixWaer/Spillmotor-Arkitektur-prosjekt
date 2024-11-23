#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Camera.h"
#include "../Engine/EventCallback.h"
#include "../GameObject Components/SphereCollision.h"

class BasicSphere;
class Terrain;

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
	void input_LMouseFunction();
	void input_RMouseFunction();
	void input_ESCFunction();
	void collision_Function(GameObject* otherGameObject, glm::vec3 hitPosition);

	Event* W_InputEvent = nullptr;
	Event* A_InputEvent = nullptr;
	Event* S_InputEvent = nullptr;
	Event* D_InputEvent = nullptr;
	Event* LM_InputEvent = nullptr;
	Event* RM_InputEvent = nullptr;
	Event* ESC_InputEvent = nullptr;
	Event* CollisionEvent = nullptr;

	Camera ActiveCamera;
	Terrain* TriangulatedTerrain = nullptr;
	std::vector<BasicSphere*> Balls;
};


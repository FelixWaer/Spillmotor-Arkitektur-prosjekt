#include "EngineCamera.h"

#include <iostream>

#include "../Engine/EngineManager.h"
#include "../Engine/Input.h"
#include "BasicSphere.h"
#include "PunktSky.h"
#include "LightCube.h"

void EngineCamera::game_Start()
{
	//Set the Camera position
	ActiveCamera.update_CameraPosition(glm::vec3(0.f, 100.f, 0.f));

	//Set the Camera speed
	ActiveCamera.set_CameraSpeed(50.f);
	EngineManager::get()->get_ActiveScene()->set_SceneCamera(&ActiveCamera);

	PunktSkyModel = new PunktSky;
	PunktSkyModel->init_GameObject();

	Light = new LightCube;
	Light->init_GameObject();
	Light->set_GameObjectPosition(get_GameObjectPosition());

	//InputEvent = new EventCallback(this, &EngineCamera::test_Function);
	W_InputEvent = make_Event(this, &EngineCamera::input_WFunction);
	A_InputEvent = make_Event(this, &EngineCamera::input_AFunction);
	S_InputEvent = make_Event(this, &EngineCamera::input_SFunction);
	D_InputEvent = make_Event(this, &EngineCamera::input_DFunction);
	One_InputEvent = make_Event(this, &EngineCamera::input_OneFunction);
	Two_InputEvent = make_Event(this, &EngineCamera::input_TwoFunction);
	Three_InputEvent = make_Event(this, &EngineCamera::input_ThreeFunction);
	Four_InputEvent = make_Event(this, &EngineCamera::input_FourFunction);
	Five_InputEvent = make_Event(this, &EngineCamera::input_FiveFunction);
	Six_InputEvent = make_Event(this, &EngineCamera::input_SixFunction);
	LM_InputEvent = make_Event(this, &EngineCamera::input_LMouseFunction);
	RM_InputEvent = make_Event(this, &EngineCamera::input_RMouseFunction);
	ESC_InputEvent = make_Event(this, &EngineCamera::input_ESCFunction);

	Input::bind_EventToKey(W_InputEvent, Key::W, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(A_InputEvent, Key::A, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(S_InputEvent, Key::S, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(D_InputEvent, Key::D, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(One_InputEvent, Key::One, KeyPress::OnPress);
	Input::bind_EventToKey(Two_InputEvent, Key::Two, KeyPress::OnPress);
	Input::bind_EventToKey(Three_InputEvent, Key::Three, KeyPress::OnPress);
	Input::bind_EventToKey(Four_InputEvent, Key::Four, KeyPress::OnPress);
	Input::bind_EventToKey(Five_InputEvent, Key::Five, KeyPress::OnPress);
	Input::bind_EventToKey(Six_InputEvent, Key::Six, KeyPress::OnPress);
	Input::bind_EventToKey(LM_InputEvent, Key::LMouse, KeyPress::OnPress);
	Input::bind_EventToKey(RM_InputEvent, Key::RMouse, KeyPress::OnPress);
	Input::bind_EventToKey(ESC_InputEvent, Key::ESCAPE, KeyPress::OnPress);
}

void EngineCamera::tick(float deltaTime)
{
	GameObject::tick(deltaTime);

	set_GameObjectPosition(ActiveCamera.get_CameraPosition());
	Light->set_GameObjectPosition(get_GameObjectPosition());
}

void EngineCamera::set_Terrain(Terrain* terrain)
{
	TriangulatedTerrain = terrain;
}

void EngineCamera::input_WFunction()
{
	ActiveCamera.move_CameraFront(true);
}

void EngineCamera::input_AFunction()
{
	ActiveCamera.move_CameraSide(false);
}

void EngineCamera::input_SFunction()
{
	ActiveCamera.move_CameraFront(false);
}

void EngineCamera::input_DFunction()
{
	ActiveCamera.move_CameraSide(true);
}

void EngineCamera::input_OneFunction()
{
	if (TriangulatedTerrain == nullptr)
	{
		TriangulatedTerrain = new Terrain;
		TriangulatedTerrain->init_GameObject();
	}
}

void EngineCamera::input_TwoFunction()
{
	TriangulatedTerrain->add_FrictionPoint(get_GameObjectPosition());
	MakeFriction++;
	if (MakeFriction >= 2)
	{
		TriangulatedTerrain->create_Friction();
	}
}

void EngineCamera::input_ThreeFunction()
{
	int xPos = get_GameObjectPosition().x - 50.f;
	int zPos = get_GameObjectPosition().z - 50.f;
	float yPos = get_GameObjectPosition().y;

	BasicSphere* tempBall = nullptr;

	for (int x = xPos; x < xPos + 100; x += 5)
	{
		for (int z = zPos; z < zPos + 100; z += 5)
		{
			tempBall = new BasicSphere;

			tempBall->init_GameObject();
			tempBall->set_GameObjectPosition(glm::vec3(x, yPos, z));
			tempBall->set_Terrain(TriangulatedTerrain);

			Balls.emplace_back(tempBall);
		}
	}
}

void EngineCamera::input_FourFunction()
{
	EngineManager::get()->get_ActiveScene()->set_CollisionCheck(collisionCheck);
	std::cout << "Collision: " << collisionCheck << std::endl;
	collisionCheck = !collisionCheck;
}

void EngineCamera::input_FiveFunction()
{
	PunktSkyModel->hide_PunktSky(hidingPunktSky);
	std::cout << "Hiding PunktSky: " << hidingPunktSky << std::endl;
	hidingPunktSky = !hidingPunktSky;
}

void EngineCamera::input_SixFunction()
{
	BasicSphere::HidingBSpline = !BasicSphere::HidingBSpline;
}

void EngineCamera::input_LMouseFunction()
{
	for (BasicSphere* ball : Balls)
	{
		glm::vec3 direction = ball->get_GameObjectPosition() - get_GameObjectPosition();
		if (glm::length(direction) <= 5.f)
		{
			direction = glm::normalize(direction);

			ball->set_GameObjectVelocity(direction * 10.f);
		}
	}

}

void EngineCamera::input_RMouseFunction()
{
	if (TriangulatedTerrain == nullptr)
	{
		return;
	}

	BasicSphere* tempBall = new BasicSphere;

	tempBall->init_GameObject();
	tempBall->set_GameObjectPosition(get_GameObjectPosition() + ActiveCamera.get_CameraTarget()*2.f);
	tempBall->set_Terrain(TriangulatedTerrain);

	Balls.emplace_back(tempBall);
}

void EngineCamera::input_ESCFunction()
{
	EngineManager::get()->get_ActiveWindow().close_Window();
}

void EngineCamera::collision_Function(GameObject* otherGameObject, glm::vec3 hitPosition)
{

}

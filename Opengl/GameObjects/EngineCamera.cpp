#include "EngineCamera.h"

#include <iostream>

#include "../Engine/EngineManager.h"
#include "../Engine/Input.h"
#include "BasicSphere.h"

void EngineCamera::game_Start()
{
	//Set the Camera position
	ActiveCamera.update_CameraPosition(glm::vec3(0.f, 100.f, 0.f));

	//Set the Camera speed
	ActiveCamera.set_CameraSpeed(50.f);
	EngineManager::get()->get_ActiveScene()->set_SceneCamera(&ActiveCamera);

	//InputEvent = new EventCallback(this, &EngineCamera::test_Function);
	W_InputEvent = make_Event(this, &EngineCamera::input_WFunction);
	A_InputEvent = make_Event(this, &EngineCamera::input_AFunction);
	S_InputEvent = make_Event(this, &EngineCamera::input_SFunction);
	D_InputEvent = make_Event(this, &EngineCamera::input_DFunction);
	One_InputEvent = make_Event(this, &EngineCamera::input_OneFunction);
	Two_InputEvent = make_Event(this, &EngineCamera::input_TwoFunction);
	LM_InputEvent = make_Event(this, &EngineCamera::input_LMouseFunction);
	RM_InputEvent = make_Event(this, &EngineCamera::input_RMouseFunction);
	ESC_InputEvent = make_Event(this, &EngineCamera::input_ESCFunction);

	Input::bind_EventToKey(W_InputEvent, Key::W, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(A_InputEvent, Key::A, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(S_InputEvent, Key::S, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(D_InputEvent, Key::D, KeyPress::WhileHeldDown);
	Input::bind_EventToKey(One_InputEvent, Key::One, KeyPress::OnPress);
	Input::bind_EventToKey(Two_InputEvent, Key::Two, KeyPress::OnPress);
	Input::bind_EventToKey(LM_InputEvent, Key::LMouse, KeyPress::OnPress);
	Input::bind_EventToKey(RM_InputEvent, Key::RMouse, KeyPress::OnPress);
	Input::bind_EventToKey(ESC_InputEvent, Key::ESCAPE, KeyPress::OnPress);
}

void EngineCamera::tick(float deltaTime)
{
	GameObject::tick(deltaTime);

	set_GameObjectPosition(ActiveCamera.get_CameraPosition());
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

void EngineCamera::input_LMouseFunction()
{
	//BasicSphere* tempBall = new BasicSphere;

	//tempBall->init_GameObject();

	//tempBall->set_GameObjectPosition(get_GameObjectPosition());
	//tempBall->set_GameObjectVelocity(ActiveCamera.get_CameraTarget()*50.f);
	//tempBall->enable_Gravity(true);

	//Balls.emplace_back(tempBall);

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
	//tempBall->set_GameObjectVelocity(ActiveCamera.get_CameraTarget()*50.f);
	//tempBall->Mass = 50.f;
	//tempBall->enable_Gravity(true);

	Balls.emplace_back(tempBall);
}

void EngineCamera::input_ESCFunction()
{
	EngineManager::get()->get_ActiveWindow().close_Window();
}

void EngineCamera::collision_Function(GameObject* otherGameObject, glm::vec3 hitPosition)
{
	if (otherGameObject == nullptr)
	{
		return;
	}

	if (otherGameObject->has_Tag("Ball") == true)
	{

	}
}

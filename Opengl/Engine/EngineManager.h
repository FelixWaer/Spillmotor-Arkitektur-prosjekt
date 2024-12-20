#pragma once

#include "../Rendering/RenderManager.h"
#include "SceneManager.h"
#include "WindowManager.h"

class EngineManager
{
public:
	EngineManager(EngineManager& other) = delete;
	~EngineManager();

	static EngineManager* get();

	void init_Engine();
	void run_Engine();

	void set_ActiveScene(SceneManager* newActiveScene);
	SceneManager* get_ActiveScene();
	WindowManager& get_ActiveWindow();

	float get_DeltaTime();
	int get_WindowWidth();
	int get_WindowHeight();
	float get_AspectRatio();
	Mesh* get_Mesh(const std::string& meshName);
	Mesh* create_Mesh(const std::string& meshName);
private:
	EngineManager() = default;

	inline static EngineManager* TheEngineManager = nullptr;

	SceneManager* ActiveScene = nullptr;
	WindowManager ActiveWindow;
	RenderManager RenderManager;

	float DeltaTime = 0.f;
	float LastFrame = 0.f;
	float FrameTimer = 0.f;
	int counter = 0;

	void tick_Engine();
};

#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"
#include "../GameObject Components/SphereCollision.h"
#include "../GameObjects/BSplineTracker.h"

class Terrain;

class BasicSphere : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;
	void set_Terrain(Terrain* terrain);

	float Mass = 1.f;
	bool CanUpdateVelocity = false;

	static inline bool HidingBSpline = false;
private:
	Model Cube;
	SphereCollision collider;
	Event* PhysicsEvent = nullptr;
	Event* GravityEvent = nullptr;
	Terrain* TriangulatedTerrain = nullptr;
	BSplineTracker BSpline;

	float BSplineTimer = 0.f;
	glm::vec3 newVelocity = glm::vec3(0.f);

	void collision_Physics(GameObject* otherGameObject, glm::vec3 hitPosition);
	void bounce_BallOfGround(glm::vec3& surfaceNormal);
	void turn_OnGravityButton();
};


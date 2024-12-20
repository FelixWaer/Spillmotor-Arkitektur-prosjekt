#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

class GameObject
{
public:
	GameObject() = default;

	void init_GameObject();
	virtual void tick(float deltaTime);
	virtual void game_Start();
	
	void set_GameObjectPosition(glm::vec3 newPosition);
	glm::vec3& get_GameObjectPosition();
	glm::vec3* get_GameObjectPositionPtr();
	void set_Acceleration(glm::vec3 newAcceleration);
	glm::vec3& get_Acceleration();
	void set_GameObjectVelocity(const glm::vec3& newVelocity);
	glm::vec3& get_GameObjectVelocity();
	void set_GameObjectFront(const glm::vec3& newFront);
	glm::vec3& get_GameObjectFront();
	void set_GameObjectSpeed(float newSpeed);
	float& get_GameObjectSpeed();
	float& get_Mass();
	void add_Tag(const std::string& tag);
	bool has_Tag(const std::string& tag);
	void enable_Gravity(bool enable);
protected:
	glm::vec3 GameObjectPosition = glm::vec3(0.f);
	glm::vec3 Acceleration = glm::vec3(0.f);
	glm::vec3 GameObjectVelocity = glm::vec3(0.f);
	glm::vec3 GameObjectFront = glm::vec3(0.f);
	float GameObjectSpeed = 0.f;
	bool GravityEnabled = false;
	float Mass = 1.f;

private:
	std::vector<std::string> tags;
};
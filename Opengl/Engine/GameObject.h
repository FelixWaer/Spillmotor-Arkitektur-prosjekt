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
	void set_GameObjectVelocity(const glm::vec3& newVelocity);
	glm::vec3& get_GameObjectVelocity();
	void set_GameObjectFront(const glm::vec3& newFront);
	glm::vec3& get_GameObjectFront();
	void set_GameObjectSpeed(float newSpeed);
	float get_GameObjectSpeed();
	void add_Tag(const std::string& tag);
	bool has_Tag(const std::string& tag);
protected:
	glm::vec3 GameObjectPosition = glm::vec3(0.f);
	glm::vec3 GameObjectVelocity = glm::vec3(0.f);
	glm::vec3 GameObjectFront = glm::vec3(0.f);
	float GameObjectSpeed = 0.f;

private:
	std::vector<std::string> tags;
};
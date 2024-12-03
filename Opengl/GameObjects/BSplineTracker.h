#pragma once
#include "../Engine/GameObject.h"
#include "../Rendering/Model.h"

class BSplineTracker : public GameObject
{
public:
	void game_Start() override;
	void tick(float deltaTime) override;

	void generate_BSpline();
	void add_ControlPoint(glm::vec3& newControlPoint);
	void hide_BSpline(bool hidingBSpline);
private:
	Model BSplineModel;

	std::vector<glm::vec3> ControlPoints;
	std::vector<float> KnotVector;
	int Degree = 2;
	int numberOfPoints = 100;
	std::string bSplineName;

	void generate_KnotVector(int NumberOfControlPoints);
	glm::vec3 calculate_BSpline(float t);
	float blending_Function(int i, int degree, float t);

	static inline int Counter = 0;
};


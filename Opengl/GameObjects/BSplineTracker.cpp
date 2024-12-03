#include "BSplineTracker.h"

#include "../Engine/EngineManager.h"
#include "glad/glad.h"

void BSplineTracker::game_Start()
{
	set_GameObjectPosition(glm::vec3(0.f));

	bSplineName = "BSpline" + std::to_string(Counter);
	Counter++;
	Mesh* tempPtr = EngineManager::get()->create_Mesh(bSplineName);

	tempPtr->bind_Buffer(GL_DYNAMIC_DRAW);

	BSplineModel.init_Model();
	BSplineModel.attach_ToGameObject(this);
	BSplineModel.set_ModelMesh(bSplineName);
	BSplineModel.hide_Model(false);
	BSplineModel.set_ModelAsLine();
}

void BSplineTracker::tick(float deltaTime)
{
}

void BSplineTracker::generate_BSpline()
{
	if (ControlPoints.size() < 4)
	{
		return;
	}

	generate_KnotVector(ControlPoints.size());

	std::vector<Vertex>& bSplinePoints = EngineManager::get()->get_Mesh(bSplineName)->Vertices;
	bSplinePoints.clear();

	float increment = 1.f  / (150 - 1);

	for (float t = 0; t <= 1; t += increment) {
		glm::vec3 point_on_curve = calculate_BSpline(t);
		bSplinePoints.emplace_back(point_on_curve, glm::vec3(1.f, 0.f, 0.f));
	}

	EngineManager::get()->get_Mesh(bSplineName)->rebind_Buffer(GL_DYNAMIC_DRAW);
}

void BSplineTracker::add_ControlPoint(glm::vec3& newControlPoint)
{
	//EngineManager::get()->get_Mesh(bSplineName)->Vertices.emplace_back(newControlPoint, glm::vec3(1.f, 0.f, 0.f));
	ControlPoints.emplace_back(newControlPoint);
}

void BSplineTracker::hide_BSpline(bool hidingBSpline)
{
	BSplineModel.hide_Model(hidingBSpline);
}

void BSplineTracker::generate_KnotVector(int NumberOfControlPoints)
{
	int numberOfKnots = NumberOfControlPoints + Degree + 1;

	std::vector<float> knots(numberOfKnots);

	// Set the first and last 'degree+1' knots
	for (int i = 0; i <= Degree; i++) {
		knots[i] = 0.0f;
		knots[numberOfKnots - 1 - i] = 1.0f;
	}

	// Set the remaining knots
	for (int i = Degree + 1; i < numberOfKnots - Degree - 1; i++) {
		knots[i] = 1.0f * (i - Degree) / (NumberOfControlPoints - Degree);
	}

	KnotVector = knots;
}

glm::vec3 BSplineTracker::calculate_BSpline(float t)
{
	glm::vec3 point(0.0f);

	for (int i = 0; i < ControlPoints.size(); i++) {
		float blendingFunctionValue = blending_Function(i, Degree,  t);
		point += blendingFunctionValue * ControlPoints[i];
	}

	return point;
}

float BSplineTracker::blending_Function(int i, int degree, float t)
{
	if (degree == 0) {
		return (KnotVector[i] <= t && t < KnotVector[i + 1]) ? 1.0f : 0.0f;
	}
	else {
		float a = 0.0f;
		if (i + degree < KnotVector.size() && KnotVector[i + degree] != KnotVector[i]) {
			a = (t - KnotVector[i]) / (KnotVector[i + degree] - KnotVector[i]);
		}

		float b = 0.0f;
		if (i + degree + 1 < KnotVector.size() && KnotVector[i + degree + 1] != KnotVector[i + 1]) {
			b = (KnotVector[i + degree + 1] - t) / (KnotVector[i + degree + 1] - KnotVector[i + 1]);
		}

		return a * blending_Function(i, degree - 1, t) + b * blending_Function(i + 1, degree - 1, t);
	}
}

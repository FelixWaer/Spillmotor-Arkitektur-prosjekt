#pragma once
#include <glm/glm.hpp>

struct Vertex;

namespace FLXMath
{
	bool calculate_PointOnTriangle(glm::vec3& x, const glm::vec3& P, const glm::vec3& Q, const glm::vec3& R, glm::vec3& aVector, float& height);

	glm::vec3 calculate_AccelerationVector(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

	float calculate_FrictionForce(const glm::vec3& normal, float frictionValue);

	float calculate_SurfaceAngle(glm::vec3 surfaceNormal);

	void calculate_TriangleNormal(Vertex& vertexA, Vertex& vertexB, Vertex& vertexC);

	inline float calculate_Normal(const glm::vec3& AB, const glm::vec3& AC);

	inline float calculate_Normal(const glm::vec3&& AB, const glm::vec3&& AC);

	float calculate_AngleBetweenVectors(const glm::vec3& vectorA, const glm::vec3& vectorB);

	inline void switch_YZ(glm::vec3& vector);
}

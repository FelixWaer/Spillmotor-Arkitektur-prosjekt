#pragma once
#include <glm/glm.hpp>

struct Vertex;

namespace FLXMath
{
	bool calculate_PointOnTriangle(glm::vec3& x, const glm::vec3& P, const glm::vec3& Q, const glm::vec3& R, const glm::vec3& position);

	void calculate_TriangleNormal(Vertex& vertexA, Vertex& vertexB, Vertex& vertexC);

	inline float calculate_Normal(const glm::vec3& AB, const glm::vec3& AC);

	inline float calculate_Normal(const glm::vec3&& AB, const glm::vec3&& AC);

	float calculate_AngleBetweenVectors(const glm::vec3& vectorA, const glm::vec3& vectorB);

	inline void switch_YZ(glm::vec3& vector);
}

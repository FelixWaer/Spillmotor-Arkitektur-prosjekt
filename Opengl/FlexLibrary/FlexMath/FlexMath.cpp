#include "FlexMath.h"

#include "../../Rendering/MeshData.h"

namespace FLXMath
{
	bool calculate_PointOnTriangle(glm::vec3& x, const glm::vec3& P, const glm::vec3& Q, const glm::vec3& R, glm::vec3& aVector)
	{
	/*	tempVector.z = x.y;
		tempVector.y = x.z;*/
		//tempVector.y -= 0.5f;

		float A = calculate_Normal(Q - P, R - P);

		float U = calculate_Normal((Q - x), (R - x)) / A;
		float V = calculate_Normal((R - x), (P - x)) / A;
		float W = 1.f - U - V;

		if (U >= 0 && V >= 0 && W >= 0)
		{
			float triangleHeight = U * (P.y) + V * (Q.y) + W * (R.y);
			x.y = triangleHeight + 1.f;

			aVector = calculate_AccelerationVector(P, Q, R);

			return true;
		}
		else
		{
			return false;
		}
	}

	glm::vec3 calculate_AccelerationVector(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		//normal
		glm::vec3 n = glm::cross(a - b, c - a);
		//gravity
		float g = 9.81f;

		return glm::vec3(n.y * n.x * g, (n.y * n.y * g) - 1.f, n.y * n.z * g);
	}

	void calculate_TriangleNormal(Vertex& vertexA, Vertex& vertexB, Vertex& vertexC)
	{
		glm::vec3 normal = glm::cross(vertexB.Position - vertexA.Position, vertexC.Position - vertexA.Position);

		vertexA.Normal += glm::normalize(normal);
		vertexB.Normal += glm::normalize(normal);
		vertexC.Normal += glm::normalize(normal);
	}

	float calculate_Normal(const glm::vec3& AB, const glm::vec3& AC)
	{
		return AB[0] * AC[2] - AC[0] * AB[2];
	}

	float calculate_Normal(const glm::vec3&& AB, const glm::vec3&& AC)
	{
		return AB[0] * AC[2] - AC[0] * AB[2];
	}

	float calculate_AngleBetweenVectors(const glm::vec3& vectorA, const glm::vec3& vectorB)
	{
		float dotProduct = glm::dot(vectorA, vectorB);
		float magnitudeA = static_cast<float>(sqrt(pow(vectorA.x, 2) + pow(vectorA.y, 2) + pow(vectorA.z, 2)));
		float magnitudeB = static_cast<float>(sqrt(pow(vectorB.x, 2) + pow(vectorB.y, 2) + pow(vectorB.z, 2)));

		return cos(dotProduct / magnitudeA * magnitudeB);
	}

	void switch_YZ(glm::vec3& vector)
	{
		float tempValue = vector.z;
		vector.z = vector.y;
		vector.y = tempValue;
	}

}

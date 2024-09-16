#pragma once

#include <glm/glm.hpp>

class Mesh;

namespace FLXModel
{
	void create_Cube(Mesh& mesh, const glm::vec3& color, bool reverseNormals = false);

	void create_SphereLines(Mesh& lineModel, float radius, const glm::vec3& color);

	void create_BoxLines(Mesh& lineModel, float height, float width, float depth, const glm::vec3& color);

	void create_Ball(Mesh& mesh, const glm::vec3& color, int numberOfSubDiv);
}
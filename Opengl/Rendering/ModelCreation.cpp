#include "ModelCreation.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "MeshData.h"
#include "Mesh.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"
#include "../FlexLibrary/FlexTimer/Flextimer.h"

namespace FLXModel
{
	void create_Cube(Mesh& mesh, const glm::vec3& color, bool reverseNormals)
	{
		mesh.Vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f), color, glm::vec2(0.0f, 1.0f));
		mesh.Vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f), color, glm::vec2(0.0f, 0.0f));
		mesh.Vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f), color, glm::vec2(1.0f, 1.0f));
		mesh.Vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f), color, glm::vec2(1.0f, 0.0f));

		mesh.Vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f), color, glm::vec2(1.0f, 1.0f));
		mesh.Vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f), color, glm::vec2(1.0f, 0.0f));
		mesh.Vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f), color, glm::vec2(0.0f, 0.0f));
		mesh.Vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f), color, glm::vec2(0.0f, 1.0f));

		mesh.Triangles.emplace_back(0, 1, 2);

		mesh.Triangles.emplace_back(2, 3, 0);

		mesh.Triangles.emplace_back(0, 4, 5);

		mesh.Triangles.emplace_back(5, 1, 0);

		mesh.Triangles.emplace_back(3, 2, 6);

		mesh.Triangles.emplace_back(6, 7, 3);

		mesh.Triangles.emplace_back(1, 5, 6);

		mesh.Triangles.emplace_back(6, 2, 1);

		mesh.Triangles.emplace_back(0, 3, 7);

		mesh.Triangles.emplace_back(7, 4, 0);

		mesh.Triangles.emplace_back(4, 7, 6);

		mesh.Triangles.emplace_back(6, 5, 4);

		for (const Triangle& triangle : mesh.Triangles)
		{
			if (reverseNormals == true)
			{
				FLXMath::calculate_TriangleNormal(mesh.Vertices[triangle.FirstIndex],
					mesh.Vertices[triangle.ThirdIndex], mesh.Vertices[triangle.SecondIndex]);
			}
			else
			{
				FLXMath::calculate_TriangleNormal(mesh.Vertices[triangle.FirstIndex],
					mesh.Vertices[triangle.SecondIndex], mesh.Vertices[triangle.ThirdIndex]);
			}
		}

		mesh.bind_Buffer(GL_STATIC_DRAW);
	}

	void create_SphereLines(Mesh& lineModel, float radius, const glm::vec3& color)
	{
		float degrees = 22.5f;
		glm::vec3 position;
		for (int i = 0; i < 18; i++)
		{
			position = glm::vec3(radius * sin(glm::radians(degrees * i)), radius * cos(glm::radians(degrees * i)), 0.f);
			lineModel.Vertices.emplace_back(position, glm::vec3(1.f), color, glm::vec2(0.0f));
		}
		for (int i = 0; i < 21; i++)
		{
			position = glm::vec3(0.f, radius * cos(glm::radians(degrees * i)), radius * sin(glm::radians(degrees * i)));
			lineModel.Vertices.emplace_back(position, glm::vec3(1.f), color, glm::vec2(0.0f));
		}
		for (int i = 0; i < 18; i++)
		{
			position = glm::vec3(radius * sin(glm::radians(degrees * i)), 0.f, radius * cos(glm::radians(degrees * i)));
			lineModel.Vertices.emplace_back(position, glm::vec3(1.f), color, glm::vec2(0.0f));
		}

		lineModel.bind_Buffer(GL_STATIC_DRAW);
	}

	void create_BoxLines(Mesh& lineModel, float height, float width, float depth, const glm::vec3& color)
	{
		lineModel.Vertices.emplace_back(glm::vec3(width / 2, height / 2, depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));
		lineModel.Vertices.emplace_back(glm::vec3(-width / 2, height / 2, depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));

		lineModel.Vertices.emplace_back(glm::vec3(width / 2, -height / 2, depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));
		lineModel.Vertices.emplace_back(glm::vec3(-width / 2, -height / 2, depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));

		lineModel.Vertices.emplace_back(glm::vec3(width / 2, height / 2, -depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));
		lineModel.Vertices.emplace_back(glm::vec3(-width / 2, height / 2, -depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));

		lineModel.Vertices.emplace_back(glm::vec3(width / 2, -height / 2, -depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));
		lineModel.Vertices.emplace_back(glm::vec3(-width / 2, -height / 2, -depth / 2), glm::vec3(1.f), color, glm::vec2(0.0f));

		lineModel.bind_Buffer(GL_STATIC_DRAW);
	}

	void subDivide_Triangle(Mesh& model, int indexV1, int indexV2, int indexV3, int numberOfSubDiv)
	{
		if (numberOfSubDiv > 0)
		{
			Vertex tempVertex1;
			tempVertex1.Position = glm::normalize(model.Vertices[indexV1].Position + model.Vertices[indexV2].Position);
			model.Vertices.emplace_back(tempVertex1);
			int indexTempV1 = static_cast<int>(model.Vertices.size() - 1);

			Vertex tempVertex2;
			tempVertex2.Position = glm::normalize(model.Vertices[indexV1].Position + model.Vertices[indexV3].Position);
			model.Vertices.emplace_back(tempVertex2);
			int indexTempV2 = static_cast<int>(model.Vertices.size() - 1);

			Vertex tempVertex3;
			tempVertex3.Position = glm::normalize(model.Vertices[indexV3].Position + model.Vertices[indexV2].Position);
			model.Vertices.emplace_back(tempVertex3);
			int indexTempV3 = static_cast<int>(model.Vertices.size() - 1);

			subDivide_Triangle(model, indexV1, indexTempV1, indexTempV2, numberOfSubDiv - 1);
			subDivide_Triangle(model, indexV3, indexTempV2, indexTempV3, numberOfSubDiv - 1);
			subDivide_Triangle(model, indexV2, indexTempV3, indexTempV1, numberOfSubDiv - 1);
			subDivide_Triangle(model, indexTempV3, indexTempV2, indexTempV1, numberOfSubDiv - 1);
		}
		else
		{
			model.Triangles.emplace_back(indexV1, indexV2, indexV3);
		}
	}

	void create_Ball(Mesh& mesh, const glm::vec3& color, int numberOfSubDiv)
	{
		int triangleAmount = 8 * std::pow(4, numberOfSubDiv);

		mesh.Vertices.reserve(triangleAmount - 2);
		mesh.Triangles.reserve(triangleAmount);

		mesh.Vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), color);
		mesh.Vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), color);
		mesh.Vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), color);
		mesh.Vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), color);
		mesh.Vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), color);
		mesh.Vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), color);

		subDivide_Triangle(mesh, 0, 1, 2, numberOfSubDiv);
		subDivide_Triangle(mesh, 0, 2, 3, numberOfSubDiv);
		subDivide_Triangle(mesh, 0, 3, 4, numberOfSubDiv);
		subDivide_Triangle(mesh, 0, 4, 1, numberOfSubDiv);
		subDivide_Triangle(mesh, 5, 2, 1, numberOfSubDiv);
		subDivide_Triangle(mesh, 5, 3, 2, numberOfSubDiv);
		subDivide_Triangle(mesh, 5, 4, 3, numberOfSubDiv);
		subDivide_Triangle(mesh, 5, 1, 4, numberOfSubDiv);

		for (const Triangle& triangle : mesh.Triangles)
		{
			FLXMath::calculate_TriangleNormal(mesh.Vertices[triangle.FirstIndex],
				mesh.Vertices[triangle.SecondIndex], mesh.Vertices[triangle.ThirdIndex]);
		}
		for (Vertex& vertex : mesh.Vertices)
		{
			vertex.Color = color;
		}

		mesh.bind_Buffer(GL_STATIC_DRAW);
	}

	float basisFunction(int i, int p, float t, const std::vector<float>& knotVector) {
		if (p == 0) {
			return (t >= knotVector[i] && t < knotVector[i + 1]) ? 1.0 : 0.0;
		}
		else {
			float left = (t - knotVector[i]) / (knotVector[i + p] - knotVector[i]);
			float right = (knotVector[i + p + 1] - t) / (knotVector[i + p + 1] - knotVector[i + 1]);

			float leftBasis = (knotVector[i + p] != knotVector[i]) ? left * basisFunction(i, p - 1, t, knotVector) : 0.0;
			float rightBasis = (knotVector[i + p + 1] != knotVector[i + 1]) ? right * basisFunction(i + 1, p - 1, t, knotVector) : 0.0;

			return leftBasis + rightBasis;
		}
	}

	glm::vec3 evaluate_BiquadraticBSplineSurface(
		float u, float v,
		const std::vector<std::vector<glm::vec3>>& controlPoints,
		const std::vector<float>& uKnotVector,
		const std::vector<float>& vKnotVector
	) {
		int n = controlPoints.size();    // Number of control points in the u-direction
		int m = controlPoints[0].size(); // Number of control points in the v-direction
		int d = 2; 

		glm::vec3 surfacePoint(0.0f); // Initialize to zero (assuming 3D control points)

		// Iterate over control points and sum the tensor product of the B-spline basis functions
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				float basisU = basisFunction(i, d, u, uKnotVector);
				float basisV = basisFunction(j, d, v, vKnotVector);
				float weight = basisU * basisV;

				// Add contribution from control point
				surfacePoint += weight * controlPoints[i][j];
			}
		}

		return surfacePoint;
	}

	void generate_SurfacePointsAndTriangles(
		int gridResolutionU, int gridResolutionV,
		const std::vector<std::vector<glm::vec3>>& controlPoints,
		const std::vector<float>& uKnotVector,
		const std::vector<float>& vKnotVector,
		std::vector<glm::vec3>& surfacePoints,
		std::vector<Triangle>& indices
	) {
		// Clear previous data
		surfacePoints.clear();
		indices.clear();

		// Loop over the parameter space to evaluate the surface points
		for (int i = 0; i < gridResolutionU; ++i) {
			for (int j = 0; j < gridResolutionV; ++j) {
				// Map i, j to the parametric space [0, 2] (knot vector range)
				float u = 2.0 * i / (gridResolutionU - 1); // Normalize to [0, 2] for uniform knots
				float v = 2.0 * j / (gridResolutionV - 1);

				// Evaluate the surface at the current (u, v)
				glm::vec3 surfacePoint = evaluate_BiquadraticBSplineSurface(u, v, controlPoints, uKnotVector, vKnotVector);

				// Store the surface point
				surfacePoints.push_back(surfacePoint);
			}
		}

		// Generate indices for the triangles
		for (int i = 0; i < gridResolutionU - 1; ++i) {
			for (int j = 0; j < gridResolutionV - 1; ++j) {
				// Calculate indices of the four corners of the current quad
				int topLeft = i * gridResolutionV + j;
				int topRight = topLeft + 1;
				int bottomLeft = (i + 1) * gridResolutionV + j;
				int bottomRight = bottomLeft + 1;

				// First triangle (top-left, bottom-left, bottom-right)
				indices.emplace_back(topLeft, bottomLeft, bottomRight);

				// Second triangle (top-left, bottom-right, top-right)
				indices.emplace_back(topLeft, bottomRight, topRight);
			}
		}
	}

	void create_BSplineSurface(Mesh& surface, glm::vec3 color)
	{
		std::vector<std::vector<glm::vec3>> controlPoints = {
		{glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(3.0, 0.0, 0.0)},
		{glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 2.0), glm::vec3(2.0, 1.0, 2.0), glm::vec3(3.0, 1.0, 0.0)},
		{glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0, 2.0, 0.0), glm::vec3(2.0, 2.0, 0.0), glm::vec3(3.0, 2.0, 0.0)},
		};

		std::vector<float> uKnotVector = { 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 4.0, 4.0 };
		std::vector<float> vKnotVector = { 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0 };

		int gridResolutionU = 20;
		int gridResolutionV = 20;
		std::vector<glm::vec3> surfacePoints;

		generate_SurfacePointsAndTriangles(gridResolutionU, gridResolutionV, controlPoints, uKnotVector, vKnotVector, surfacePoints, surface.Triangles);

		for (glm::vec3& point : surfacePoints)
		{
			surface.Vertices.emplace_back(point, color);
		}
		for (const Triangle& triangle : surface.Triangles)
		{
			FLXMath::calculate_TriangleNormal(surface.Vertices[triangle.FirstIndex],
				surface.Vertices[triangle.SecondIndex], surface.Vertices[triangle.ThirdIndex]);
		}

		surface.bind_Buffer(GL_STATIC_DRAW);
	}
	glm::vec3 triangulate_Terrain(Mesh& pointCloud, Mesh& triangulatedMesh)
	{
		FlexTimer timer("Triangulating Part");

		int Precision = 6;
		int PrecisionSize = 1 << Precision;
		int resolution = 100;
		std::cout << "Precision Size:" << PrecisionSize << std::endl;

		int xLength = static_cast<int>(pointCloud.MaxSize.x * resolution);
		int zLength = static_cast<int>(pointCloud.MaxSize.z * resolution);

		xLength >>= Precision;
		xLength++;
		zLength >>= Precision;
		zLength++;

		std::cout << "Why is this so big!: " << xLength << std::endl;
		std::cout << "Why is this so big!: " << zLength << std::endl;

		triangulatedMesh.Vertices.resize(xLength * zLength);

		std::vector<int> counter;
		counter.resize(xLength * zLength);

		glm::ivec2 Coords(0);

		for (int x = 0; x < xLength; x++)
		{
			for (int z = 0; z < zLength; z++)
			{
				triangulatedMesh.Vertices[x + (z * xLength)].Position = glm::vec3(x * PrecisionSize, 0.f, z * PrecisionSize) * 0.01f;
			}
		}

		for (Vertex& vertex : pointCloud.Vertices)
		{
			Coords.x = static_cast<int>(vertex.Position.x * resolution);
			Coords.y = static_cast<int>(vertex.Position.z * resolution);
			Coords >>= Precision;

			int index = Coords.x + (Coords.y * xLength);

			counter[index]++;

			triangulatedMesh.Vertices[index].Position.y += vertex.Position.y;
			triangulatedMesh.Vertices[index].Color += vertex.Color;
		}

		for (int i = 0; i < triangulatedMesh.Vertices.size(); i++)
		{
			if (counter[i] <= 0)
			{
				if ((i + xLength) < triangulatedMesh.Vertices.size())
				{
					if (counter[i + xLength] != 0)
					{
						triangulatedMesh.Vertices[i].Position.y += (triangulatedMesh.Vertices[i + xLength].Position.y / counter[i + xLength]);
						triangulatedMesh.Vertices[i].Color += triangulatedMesh.Vertices[i + xLength].Color / glm::vec3(counter[i + xLength]);
						counter[i]++;
					}

				}
				if (i - xLength >= 0)
				{
					triangulatedMesh.Vertices[i].Position.y += triangulatedMesh.Vertices[i - xLength].Position.y;
					triangulatedMesh.Vertices[i].Color += triangulatedMesh.Vertices[i - xLength].Color;
					counter[i]++;
				}
				if ((i % (xLength + 1)) != 0 && (i + 1) < triangulatedMesh.Vertices.size())
				{
					if (counter[i + 1] != 0)
					{
						triangulatedMesh.Vertices[i].Position.y += (triangulatedMesh.Vertices[i + 1].Position.y / counter[i + 1]);
						triangulatedMesh.Vertices[i].Color += triangulatedMesh.Vertices[i + 1].Color / glm::vec3(counter[i + 1]);
						counter[i]++;
					}
				}
				if ((i % xLength) != 0 && (i - 1) >= 0)
				{
					triangulatedMesh.Vertices[i].Position.y += (triangulatedMesh.Vertices[i - 1].Position.y);
					triangulatedMesh.Vertices[i].Color += triangulatedMesh.Vertices[i - 1].Color;
					counter[i]++;
				}

				if (counter[i] <= 0)
				{
					counter[i] = 1;
				}

				triangulatedMesh.Vertices[i].Position.y /= counter[i];
				triangulatedMesh.Vertices[i].Color /= counter[i];
			}
			else
			{

				triangulatedMesh.Vertices[i].Position.y /= counter[i];
				triangulatedMesh.Vertices[i].Color /= counter[i];
			}

			if ((i + 1) % xLength == 0 || (i + xLength) >= triangulatedMesh.Vertices.size())
			{
				continue;
			}

			triangulatedMesh.Triangles.emplace_back(i, i + xLength, (i + xLength) + 1);
			triangulatedMesh.Triangles.emplace_back((i + xLength) + 1, i + 1, i);
		}

		for (const Triangle& triangle : triangulatedMesh.Triangles)
		{
			FLXMath::calculate_TriangleNormal(triangulatedMesh.Vertices[triangle.FirstIndex],
				triangulatedMesh.Vertices[triangle.SecondIndex], triangulatedMesh.Vertices[triangle.ThirdIndex]);
		}

		std::cout << "Number of Vertices in triangulated Mesh: " << triangulatedMesh.Vertices.size() << std::endl;

		triangulatedMesh.bind_Buffer(GL_STATIC_DRAW);

		return glm::vec3(Precision, xLength, zLength);
	}
}
#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <OBJ_Loader/tiny_obj_loader.h>
#include <glad/glad.h>

#include <iostream>
#include <unordered_map>

#include "MeshData.h"
#include "../FlexLibrary/FlexMath/FlexMath.h"
#include "../FlexLibrary/FlexNet Client/FlexClient.h"
#include "../FlexLibrary/FlexTimer/Flextimer.h"

void Mesh::load_MeshObj(const std::string& filePath)
{
    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warning, &error, filePath.c_str(), 0, true))
    {
        throw std::runtime_error(warning + error);
    }

	Vertices.reserve(attribute.vertices.size() / 3);

	/*for (size_t sIndex = 0; sIndex < shapes.size(); sIndex++)
	{
		size_t index_Offset = 0;
		for (size_t fIndex = 0; fIndex < shapes[sIndex].mesh.num_face_vertices.size(); fIndex++)
		{
			size_t faceVertices = size_t(shapes[sIndex].mesh.num_face_vertices[fIndex]);

			for (size_t vIndex = 0; vIndex < faceVertices; vIndex++)
			{
				Vertex tempVertex;

				tinyobj::index_t index = shapes[sIndex].mesh.indices[index_Offset + vIndex];

				tempVertex.Position = { attribute.vertices[(3 * index.vertex_index) + 0],
									attribute.vertices[(3 * index.vertex_index) + 1],
									attribute.vertices[(3 * index.vertex_index) + 2]
				};

				if (index.normal_index >= 0)
				{
					tempVertex.Normal = { attribute.normals[(3 * index.normal_index) + 0],
										attribute.normals[(3 * index.normal_index) + 1],
										attribute.normals[(3 * index.normal_index) + 2]
					};
				}

				if (index.texcoord_index >= 0)
				{
					tempVertex.Texture = { attribute.texcoords[(2 * index.texcoord_index) + 0],
										attribute.texcoords[(2 * index.texcoord_index) + 1],
					};
				}

				Vertices.emplace_back(tempVertex);

				index_Offset += faceVertices;

				shapes[sIndex].mesh.material_ids[fIndex];
			}
		}
	}*/

	int textureCordIndex = 0;
    for (size_t i = 0; i < attribute.vertices.size(); i+=3)
    {
		Vertex tempVertex;
		tempVertex.Position = {attribute.vertices[i], attribute.vertices[i+1] , attribute.vertices[i+2] };
		//tempVertex.Color = { attribute.colors[i], attribute.colors[i + 1] , attribute.colors[i + 2] };
		tempVertex.Color = glm::vec3(0.5f);
		tempVertex.Texture = { attribute.texcoords[textureCordIndex], attribute.texcoords[textureCordIndex + 1] };
		//tempVertex.Normal = { attribute.normals[i], attribute.normals[i + 1] , attribute.normals[i + 2] };
		textureCordIndex += 2;

		Vertices.emplace_back(tempVertex);
    }

	for (auto shape = shapes.begin(); shape < shapes.end(); ++shape)
	{
		std::vector<tinyobj::index_t>& indices = shape->mesh.indices;
		std::vector<int>& material_Ids = shape->mesh.material_ids;

		for (size_t index = 0; index < material_Ids.size(); index++)
		{
			Triangle tempTriangle(indices[3 * index].vertex_index, indices[3 * index + 1].vertex_index, indices[3 * index + 2].vertex_index);
			Triangles.emplace_back(tempTriangle);
		}
	}

    for (const Triangle& triangle : Triangles)
    {
		FLXMath::calculate_TriangleNormal(Vertices[triangle.FirstIndex], Vertices[triangle.SecondIndex], Vertices[triangle.ThirdIndex]);
    }

    bind_Buffer(GL_STATIC_DRAW);
}

void Mesh::load_MeshTxt(const std::string& filePath, Mesh& triangulatedMesh)
{
	std::ifstream file;
	file.open(filePath);

	if (file.is_open() == false)
	{
		std::cout << "could not open model file!" << std::endl;
		return;
	}

	char dataType = 'O';
	float vectorSize = 0;
	std::string line;

	uint32_t xPos;
	uint32_t yPos;

	glm::vec3 minValue = glm::vec3(FLT_MAX);
	glm::vec3 maxValue = glm::vec3(FLT_MIN);

	while (file.eof() == false)
	{
		file >> std::ws;
		if (file.peek() == EOF)
		{
			break;
		}
		if (file.peek() == 'V')
		{
			std::getline(file, line);
			dataType = 'V';
			file >> vectorSize;
			Vertices.reserve(static_cast<unsigned int>(vectorSize));
			continue;
		}

		if (file.peek() == 'T')
		{
			std::getline(file, line);
			dataType = 'T';
			file >> vectorSize;
			Triangles.reserve(static_cast<unsigned int>(vectorSize));

			continue;
		}

		switch (dataType)
		{
		case 'V':
			glm::vec3 position = glm::vec3(0.f);
			glm::vec3 normal = glm::vec3(0.f);
			glm::vec3 color = glm::vec3(1.f);
			//file >> position.x >> position.y >> position.z >> normal.x >> normal.y >> normal.z >> color.r >> color.g >> color.b;
			file >> position.x >> position.z >> position.y >> color.r >> color.g >> color.b;

			std::getline(file, line);

			//xPos = static_cast<uint32_t>(position.x);
			//yPos = static_cast<uint32_t>(position.y);

			//position.x = xPos;
			//position.y = yPos;

			minValue.x = min(minValue.x, position.x);
			minValue.y = min(minValue.y, position.y);
			minValue.z = min(minValue.z, position.z);

			maxValue.x = max(maxValue.x, position.x);
			maxValue.y = max(maxValue.y, position.y);
			maxValue.z = max(maxValue.z, position.z);

			Vertices.emplace_back(position, normal, color, glm::vec2(0.0f));
			break;

		case 'T':
			int I_1, I_2, I_3;
			file >> I_1 >> I_2 >> I_3;
			Triangles.emplace_back(I_1, I_2, I_3);
			break;

		default:
			break;
		}
	}

	FlexTimer timer("Triangulating Part");

	int Precision = 5;
	int resolution = 100;

	for (Vertex& vertex : Vertices)
	{
		vertex.Position -= minValue;
		vertex.Position.x *= resolution;
		vertex.Position.z *= resolution;
	}

	maxValue -= minValue;

	maxValue *= resolution;

	std::cout << minValue.x << " " << minValue.y << " " << minValue.z << std::endl;
	std::cout << maxValue.x << " " << maxValue.y << " " << maxValue.z << std::endl;

	int xLength = static_cast<int>(maxValue.x);
	int zLength = static_cast<int>(maxValue.z);

	xLength >>= Precision;
	xLength++;
	zLength >>= Precision;
	zLength++;

	std::cout << "Why is this so big!: " << xLength << std::endl;
	std::cout << "Why is this so big!: " << zLength << std::endl;

	triangulatedMesh.Vertices.resize(xLength * zLength);

	std::unordered_map<int, int> counter;

	glm::ivec2 Coords(0);

	
	for (int x = 0; x < xLength; x++)
	{
		for (int z = 0; z < zLength; z++)
		{
			triangulatedMesh.Vertices[x + (z * xLength)].Position = glm::vec3(x, 0.f, z);
		}
	}

	for (Vertex& vertex : Vertices)
	{
		Coords.x = static_cast<int>(vertex.Position.x);
		Coords.y = static_cast<int>(vertex.Position.z);
		Coords >>= Precision;

		int index = Coords.x + (Coords.y * xLength);

		if (counter.contains(index) == false)
		{
			counter[index] = 0;
		}
		counter[index]++;
		//triangulatedMesh.Vertices[index].Position.x += vertex.Position.x;
		triangulatedMesh.Vertices[index].Position.y += vertex.Position.y;
		//triangulatedMesh.Vertices[index].Position.z += vertex.Position.z;
		triangulatedMesh.Vertices[index].Color += vertex.Color;
	}
	
	for (int i = 0; i < triangulatedMesh.Vertices.size(); i++)
	{
		if (counter[i] == 0)
		{
			//if ((i + 1) % xLength != 0)
			//{
			//	triangulatedMesh.Vertices[i].Position.z += triangulatedMesh.Vertices[i + 1].Position.z;
			//	counter[i]++;
			//}
			if (i - xLength >= 0)
			{

				triangulatedMesh.Vertices[i].Position.y = triangulatedMesh.Vertices[i - xLength].Position.y;
				triangulatedMesh.Vertices[i].Color = triangulatedMesh.Vertices[i - xLength].Color;
				counter[i]++;
			}
		}
		else
		{
			triangulatedMesh.Vertices[i].Position.y /= counter[i];
			triangulatedMesh.Vertices[i].Color /= counter[i];
		}

		if ((i + 1) % xLength == 0  || (i + xLength) >= triangulatedMesh.Vertices.size())
		{
			continue;
		}

		triangulatedMesh.Triangles.emplace_back(i, i + xLength, (i + xLength) + 1);
		triangulatedMesh.Triangles.emplace_back((i + xLength) + 1, i + 1, i );
	}

	for (const Triangle& triangle : triangulatedMesh.Triangles)
	{
		FLXMath::calculate_TriangleNormal(triangulatedMesh.Vertices[triangle.FirstIndex], 
			triangulatedMesh.Vertices[triangle.SecondIndex], triangulatedMesh.Vertices[triangle.ThirdIndex]);
	}

	triangulatedMesh.bind_Buffer(GL_STATIC_DRAW);

	bind_Buffer(GL_STATIC_DRAW);
}

void Mesh::create_CubeMesh()
{
	Vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(0.0f, 1.0f));
	Vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(0.0f, 0.0f));
	Vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(1.0f, 1.0f));
	Vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(1.0f, 0.0f));

	Vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(1.0f, 1.0f));
	Vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(1.0f, 0.0f));
	Vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(0.0f, 0.0f));
	Vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(0.0f, 1.0f));

	Triangles.emplace_back(0, 1, 2);

	Triangles.emplace_back(2, 3, 0);

	Triangles.emplace_back(0, 4, 5);

	Triangles.emplace_back(5, 1, 0);

	Triangles.emplace_back(3, 2, 6);

	Triangles.emplace_back(6, 7, 3);

	Triangles.emplace_back(1, 5, 6);

	Triangles.emplace_back(6, 2, 1);

	Triangles.emplace_back(0, 3, 7);

	Triangles.emplace_back(7, 4, 0);

	Triangles.emplace_back(4, 7, 6);

	Triangles.emplace_back(6, 5, 4);

	for (const Triangle& triangle : Triangles)
	{
		FLXMath::calculate_TriangleNormal(Vertices[triangle.FirstIndex], Vertices[triangle.SecondIndex],
			Vertices[triangle.ThirdIndex]);
	}

	bind_Buffer(GL_STATIC_DRAW);
}

void Mesh::bind_Buffer(int drawType)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Vertices.size() * sizeof(Vertex)), Vertices.data(), drawType);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizeiptr>(Triangles.size() * sizeof(Triangle)), Triangles.data(), drawType);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24),
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)36),

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::rebind_Buffer(int drawType)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), drawType);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Triangles.size() * sizeof(Triangle), Triangles.data(), drawType);
}

void Mesh::delete_Buffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::bind_VAOBuffer()
{
	glBindVertexArray(VAO);
}

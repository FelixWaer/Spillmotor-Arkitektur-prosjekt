#pragma once
#include <string>

class Material
{
public:
	std::string ShaderName = "FlexShader";
	std::string TextureName = "cover4";

	bool HasTexture = false;
	float Shininess = 32.f;
	float SpecularStrength = 0.2f;
};


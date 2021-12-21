#pragma once

#include "Resource.h"
//#include "Shader.h"

#include <string>

class Shader;

class Material : public Resource
{
public:
	Material();
	virtual ~Material();

	void Bind(const float4x4& transform);
	void Unbind();

	void SetTexture(Texture* tex);

	inline void SetName(const std::string& newName) { name = newName; }
	inline const std::string& GetName() const { return name; }

	inline Shader* GetShader() { return shader; }
	inline void SetShader(Shader* s) { shader = s; }

public:
	float shininess;
	float3 ambientColor;

private:
	std::string name;

	//Texture* texture;
	Shader* shader;

	



};
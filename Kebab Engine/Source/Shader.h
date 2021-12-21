#pragma once

#include "Resource.h"
#include "Math/float4x4.h"
#include "Math/float2.h"

#include <unordered_map>


enum class ShaderType
{
	FRAGMENT = 0,
	VERTEX,
	GEOMETRY
};

class Shader
{
public:
	Shader(const std::string& path);
	virtual ~Shader();

	void Bind();
	void Unbind();

	inline const std::string& GetName() { return name; }

	void SetUniformBool(const std::string& name, bool b);

	void SetUniform1i(const std::string& name, int i);
	void SetUniform1f(const std::string& name, float f);

	void SetUnifromVec2f(const std::string& name, float v0, float v1);
	void SetUnifromVec2f(const std::string& name, float2 v);

	void SetUniformVec3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformVec3f(const std::string& name, float3 v);

	void SetUniformVec4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformVec4f(const std::string& name, float4 v);

	void SetUniformMatrix4f(const std::string& name, const float4x4& mat);

private:
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	bool Compile();
	std::string ReadFile();
	std::unordered_map<unsigned int, std::string> SplitShaders(const std::string& source);

	int GetUniform(const std::string& name);

private:
	std::string name;
	unsigned int rendererID;
	std::string source;
	std::string path;

	std::unordered_map<std::string, int> uniformsLocation;
};
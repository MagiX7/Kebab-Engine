#pragma once

#include "Resource.h"

#include <unordered_map>


enum class ShaderType
{
	FRAGMENT = 0,
	VERTEX,
	GEOMETRY
};

struct GLenum;

class Shader : public Resource
{
public:
	Shader(const std::string& path);
	virtual ~Shader();

	void Bind();
	void Unbind();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);


private:
	//void CreateProgram();
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	bool Compile();
	char* ReadFile();
	std::unordered_map<unsigned int, std::string> SplitShaders(const std::string& source);

private:
	unsigned int rendererID;
	std::string source;
};
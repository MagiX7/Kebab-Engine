#include "Application.h"
#include "FileSystem.h"

#include "Shader.h"


#include "GL/glew.h"
#include <assert.h>


GLenum GetShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	else if (type == "fragment")
		return GL_FRAGMENT_SHADER;
	else
		LOG_CONSOLE("Invalid Shader Type %s", type);
}



Shader::Shader(const std::string& path) : Resource(ResourceType::SHADER), rendererID(0)
{
	assetsFile = path;
	source = ReadFile();

	auto shaderSources = SplitShaders(source);
	
	CreateShader(shaderSources[0], shaderSources[1]);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

void Shader::Bind()
{
	glUseProgram(rendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
}

//void Shader::CreateProgram()
//{
//
//	
//
//}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vsource = vertexSource.c_str();
	glShaderSource(vs, 1, &vsource, NULL);
	glCompileShader(vs);

	int success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		LOG_CONSOLE("Fragment shader compilation failed: %s", infoLog);
	}

	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsource = vertexSource.c_str();
	glShaderSource(fs, 1, &vsource, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		LOG_CONSOLE("Fragment shader compilation failed: %s", infoLog);
	}

	rendererID = glCreateProgram();
	glAttachShader(rendererID, vs);
	glAttachShader(rendererID, fs);
	glLinkProgram(rendererID);

	glGetProgramiv(rendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		LOG_CONSOLE("Program Linking failed: %s", infoLog);
	}

	glDeleteProgram(vs);
	glDeleteProgram(fs);

	return 0;
}

bool Shader::Compile()
{
	return false;
}

char* Shader::ReadFile()
{
	assert(!assetsFile.empty());

	char* buffer = 0;
	app->fileSystem->Load(assetsFile.c_str(), &buffer);
	
	return buffer;
}

std::unordered_map<GLenum, std::string> Shader::SplitShaders(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;


	const char* type = "#type";
	size_t typeLen = strlen(type);

	// Declaration Start
	size_t pos = source.find(type, 0);

	while (pos != std::string::npos)
	{
		size_t end = source.find_first_of("\r\n", pos);

		assert(end != std::string::npos, "syntax error when calculating end (end of line)");

		size_t newBegin = pos + typeLen + 1;

		std::string type = source.substr(newBegin, end - newBegin);
		// Should assert the type
		

		// Next Shader
		size_t nextLine = source.find_first_not_of("\r\n", end);
		assert(nextLine != std::string::npos, "syntax error in nextLine");

		pos = source.find(type, nextLine);

		shaderSources[GetShaderTypeFromString(type)] = pos == std::string::npos ? source.substr(nextLine) : source.substr(nextLine, pos - nextLine);

	}

	return shaderSources;
}

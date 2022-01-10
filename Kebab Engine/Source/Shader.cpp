#include "Application.h"
#include "FileSystem.h"

#include "Shader.h"

#include "GL/glew.h"

#include <assert.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>





GLenum GetShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	else if (type == "fragment")
		return GL_FRAGMENT_SHADER;
	else
		LOG_CONSOLE("Invalid Shader Type %s", type);
}



Shader::Shader(const std::string& path) : path(path), rendererID(0)
{
	struct _stat nowStat;
	if (_stat(path.c_str(), &nowStat) == 0)
	{
		lastStat = nowStat;
	}

	source = ReadFile();

	auto shaderSources = SplitShaders(source);
	
	//CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
	CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);

	int s = path.find_last_of("/");
	int e = path.find_last_of(".");
	name = path.substr(s + 1);

	created = true;
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

void Shader::ReCompile()
{
	source = ReadFile();

	auto shaderSources = SplitShaders(source);

	//CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
	CreateShader(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
}

bool Shader::Refresh()
{
	struct _stat nowStat;
	if (_stat(path.c_str(), &nowStat) == 0)
	{
		if (lastStat.st_mtime != nowStat.st_mtime)
		{
			ctime_s(timeBuf, 26, &lastStat.st_mtime);
			printf("Time modified : %s", timeBuf);
			
			ReCompile();
			
			lastStat = nowStat;
			
			return true;
		}
	}

	return false;
}

char* Shader::GetLastModifiedDate()
{
	ctime_s(timeBuf, 26, &lastStat.st_mtime);
	return timeBuf;
}

void Shader::SetUniformBool(const std::string& name, bool b)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform1f(location, b);
}

void Shader::SetUniform1i(const std::string& name, int i)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform1i(location, i);
}

void Shader::SetUniform1f(const std::string& name, float f)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform1f(location, f);
}

void Shader::SetUnifromVec2f(const std::string& name, float v0, float v1)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform2f(location, v0, v1);
}

void Shader::SetUnifromVec2f(const std::string& name, float2 v)
{
	SetUnifromVec2f(name, v.x, v.y);
}

void Shader::SetUniformVec3f(const std::string& name, float v0, float v1, float v2)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform3f(location, v0, v1, v2);
}

void Shader::SetUniformVec3f(const std::string& name, float3 v)
{
	SetUniformVec3f(name, v.x, v.y, v.z);
}

void Shader::SetUniformVec4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformVec4f(const std::string& name, float4 v)
{
	SetUniformVec4f(name, v.x, v.y, v.z, v.w);
}

void Shader::SetUniformMatrix3f(const std::string& name, const float3x3& mat)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, mat.ptr());
}

void Shader::SetUniformMatrix4f(const std::string& name, const float4x4& mat)
{
	GLint location = glGetUniformLocation(rendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.ptr());
}

std::list<UniformData> Shader::GetUniforms()
{
	//std::unordered_map<std::string, float> ret;
	std::list<UniformData> ret;

	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(rendererID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	glGetProgramInterfaceiv(rendererID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	properties.push_back(GL_ARRAY_STRIDE);
	properties.push_back(GL_OFFSET);

	std::vector<GLint> values(properties.size());

	UniformData retData;
	for (int attrib = 0; attrib < numActiveUniforms; ++attrib)
	{
		glGetProgramResourceiv(rendererID, GL_UNIFORM, attrib, properties.size(),
			&properties[0], values.size(), NULL, &values[0]);

		//if (values[1] != GL_FLOAT) // If it is not a float, we continue iterating
		//	continue;

		/*if (values[0] == -1)
			continue;*/
		if (values[1] != GL_FLOAT)
		{
			continue;
		}

		nameData.resize(values[0]); // The length of the name.
		glGetProgramResourceName(rendererID, GL_UNIFORM, attrib, nameData.size(), NULL, &nameData[0]);
		
		std::string name((char*)&nameData[0], nameData.size() - 1);

		retData.name = name;
		retData.type = properties[1];
		retData.data = values[0];
		ret.push_back(retData);
		//ret[attrib] = values[attrib];
	}

	return ret;
}

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
		LOG_CONSOLE("Vertex shader compilation failed: %s", infoLog);
	}


	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsource = fragmentSource.c_str();
	glShaderSource(fs, 1, &fsource, NULL);
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
	glValidateProgram(rendererID);
	
	glGetProgramiv(rendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(rendererID, 512, NULL, infoLog);
		LOG_CONSOLE("Program Linking failed: %s", infoLog);
	}
	else
	{
		if (created)
		{
			std::string n = name = name.substr(0, name.find("."));
			LOG_CONSOLE("%s Shader refreshed!", n.c_str());
		}
		else
		{
			LOG_CONSOLE("Program Linking created successfully!");
		}

	}

	//glDetachShader(rendererID, vs);
	glDeleteShader(vs);

	//glDetachShader(rendererID, fs);
	glDeleteShader(fs);
	
	return 0;
}

bool Shader::Compile()
{
	return false;
}

std::string Shader::ReadFile()
{
	std::string ret;
	std::ifstream in(path, std::ios::in, std::ios::binary);

	if (in)
	{
		//We seek for the end of file so we can set the returning string size
		in.seekg(0, std::ios::end);
		ret.resize(in.tellg());

		//We now seek for the file beginning to put it at the string (passing the string's first position and its size)
		in.seekg(0, std::ios::beg);
		in.read(&ret[0], ret.size());

		//Finally close the file
		in.close();
	}
	
	return ret;
}

std::unordered_map<GLenum, std::string> Shader::SplitShaders(const std::string& source)
{
	std::unordered_map<GLenum, std::string> ret;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);

	while (pos != std::string::npos)
	{
		size_t lineEnding = source.find_first_of("\r\n", pos);
		//CRONOS_ASSERT(end_of_line != std::string::npos, "Shader Syntax Error");

		size_t beginPos = pos + typeTokenLength + 1;
		std::string shaderType = source.substr(beginPos, lineEnding - beginPos);
		//CRONOS_ASSERT(StringToShaderType(shaderType), "Invalid Shader Type");

		size_t nextLine = source.find_first_not_of("\r\n", lineEnding);
		pos = source.find(typeToken, nextLine);

		ret[GetShaderTypeFromString(shaderType)] = pos == std::string::npos ? source.substr(nextLine) : source.substr(nextLine, pos - nextLine);
		/*ret[GetShaderTypeFromString(shaderType)] = source.substr(nextLine,
			pos - (nextLine == std::string::npos ? source.size() - 1 : nextLine));*/
	}

	return ret;


	//std::unordered_map<GLenum, std::string> shaderSources;


	//const char* type = "#type";
	//size_t typeLen = strlen(type);

	//// Declaration Start
	//size_t pos = source.find(type, 0);

	//while (pos != std::string::npos)
	//{
	//	size_t end = source.find_first_of("\r\n", pos);

	//	if (end == std::string::npos)
	//		LOG_CONSOLE("syntax error when calculating end(end of line)");
	//	//assert(end != std::string::npos, "syntax error when calculating end (end of line)");

	//	size_t newBegin = pos + typeLen + 1;

	//	std::string type = source.substr(newBegin, end - newBegin);
	//	// Should assert the type

	//	// Next Shader
	//	size_t nextLine = source.find_first_not_of("\r\n", end);
	//	assert(nextLine != std::string::npos, "syntax error in nextLine");

	//	pos = source.find(type, nextLine);

	//	//shaderSources[GetShaderTypeFromString(type)] = pos == std::string::npos ? source.substr(nextLine) : source.substr(nextLine, pos - nextLine);
	//	shaderSources[GetShaderTypeFromString(type)] = source.substr(nextLine, pos - (nextLine == std::string::npos ? source.size() - 1 : nextLine));

	//}

	//return shaderSources;
}

int Shader::GetUniform(const std::string& name)
{
	//if()
	return 0;
}

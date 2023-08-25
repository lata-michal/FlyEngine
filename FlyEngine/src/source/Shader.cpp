#include "Shader.h"

#include "Log.h"

using namespace feng;

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_ShaderID(0)
{
	auto[vertexSource, geometrySource, fragmentSource] = ParseShader();
	m_ShaderID = CreateShader(vertexSource, geometrySource, fragmentSource);
	Bind();
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

void Shader::Bind() const
{
	glUseProgram(m_ShaderID);
}

void Shader::Unbind() const 
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int32_t v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) 
{
	glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vec)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec));
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vec)
{
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec));
}

void Shader::BindUniformBlock(const std::string& name, uint32_t index)
{
	uint32_t uniformBlockIndex = glGetUniformBlockIndex(m_ShaderID, name.c_str());
	if (index == -1)
		Log::Print("GLGETUNIFORMBLOCKINDEX FAILED. NAME: " + name + '\n');
	else
		glUniformBlockBinding(m_ShaderID, uniformBlockIndex, index);
}

std::tuple<std::string, std::string, std::string> Shader::ParseShader()
{
	std::ifstream stream(m_FilePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
	};

	std::string line;
	std::stringstream ss[3];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("geometry") != std::string::npos)
				type = ShaderType::GEOMETRY;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int32_t)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

int32_t Shader::GetUniformLocation(const std::string& name) 
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	int32_t location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
		Log::Print("WARNING: UNIFORM " + name + " DOES NOT EXIST!\n");

	m_UniformLocationCache[name] = location;

	return location;
}

uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
{
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int32_t result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int32_t length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); 
		glGetShaderInfoLog(id, length, &length, message);

		if(type == GL_VERTEX_SHADER)
			Log::Print("FAILED TO COMPILE VERTEX SHADER!\n");
		else if(type == GL_GEOMETRY_SHADER)
			Log::Print("FAILED TO COMPILE GEOMETRY SHADER!\n");
		else if(type == GL_FRAGMENT_SHADER)
			Log::Print("FAILED TO COMPILE FRAGMENT SHADER!\n");

		Log::Print(message + '\n');

		glDeleteShader(id);
		return 0;
	}

	return id;
}

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
	uint32_t program = glCreateProgram();

	if (geometryShader.empty())
	{
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	else
	{
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, gs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(gs);
		glDeleteShader(fs);
	}

	return program;
}

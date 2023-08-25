#pragma once

#include "pch.h"

namespace feng {

	class Shader
	{
	private:
		uint32_t m_ShaderID;
		std::string m_FilePath;
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int32_t v0);
		void SetUniform1f(const std::string& name, float v0);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniformVec3f(const std::string& name, const glm::vec3& vec);
		void SetUniformVec4f(const std::string& name, const glm::vec4& vec);

		void BindUniformBlock(const std::string& name, uint32_t index);
	private:
		std::tuple<std::string, std::string, std::string> ParseShader();
		int32_t GetUniformLocation(const std::string& name);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
	};

}
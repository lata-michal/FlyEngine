#pragma once

#include "pch.h"

namespace feng {

	class IBO
	{
	private:
		uint32_t m_IBO;
		uint32_t m_IndicesCount;
	public:
		IBO() = default;

		IBO(const std::vector<uint32_t>& indices)
			: m_IndicesCount(static_cast<uint32_t>(indices.size()))
		{
			glGenBuffers(1, &m_IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		}

		void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO); }
		void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

		uint32_t GetIndicesCount() const { return m_IndicesCount; }

		void Delete() const { glDeleteBuffers(1, &m_IBO); }
	};

}
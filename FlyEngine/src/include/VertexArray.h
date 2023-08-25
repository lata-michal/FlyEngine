#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace feng {

	class VAO
	{
	private:
		uint32_t m_VAO;
		uint32_t m_VerticesCount;
	public:
		VAO()
			: m_VerticesCount(0)
		{
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
		}

		void Setup(const VBO& vbo, const IBO& ibo, VertexLayout& layout)
		{
			Bind();

			vbo.Bind();
			ibo.Bind();

			for (auto& [position, size] : layout.attribSize)
			{
				glEnableVertexAttribArray(position);
				glVertexAttribPointer(position, size, GL_FLOAT, GL_FALSE, layout.stride, (const void*)layout.GetOffset(position));
			}

			m_VerticesCount = vbo.GetVerticesSize() / layout.stride;
		}

		void Setup(const VBO& vbo, VertexLayout& layout)
		{
			Bind();

			vbo.Bind();

			for (auto& [position, size] : layout.attribSize)
			{
				glEnableVertexAttribArray(position);
				glVertexAttribPointer(position, size, GL_FLOAT, GL_FALSE, layout.stride, (const void*)layout.GetOffset(position));
			}

			m_VerticesCount = vbo.GetVerticesSize() / layout.stride;
		}

		void Bind() const { glBindVertexArray(m_VAO); }
		void Unbind() const { glBindVertexArray(0); }

		uint32_t GetVerticesCount() const { return m_VerticesCount; }

		void Delete() const { glDeleteVertexArrays(1, &m_VAO); }

		inline uint32_t GetVAO() const { return m_VAO; }
	};

}
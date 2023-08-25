#pragma once

#include "Vertex.h"
#include "VertexArray.h"

namespace feng {

	class VBO
	{
	private:
		uint32_t m_VBO;
		uint32_t m_VerticesSize;
	public:
		VBO(const std::vector<Vertex>& vertices)
			: m_VerticesSize(sizeof(Vertex) * vertices.size())
		{
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		}

		VBO(const std::vector<float>& vertices)
			: m_VerticesSize(sizeof(float) * vertices.size())
		{
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);;
		}

		void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_VBO); }
		void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
		
		void SetupInstanceVBO(uint32_t vao, uint32_t index, uint32_t count, uint32_t divisor) 
		{ 
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, count * sizeof(float), (const void*)0);
			glVertexAttribDivisor(index, divisor);
		}

		void SetupInstanceVBOMat4(uint32_t vao, uint32_t index, uint32_t divisor)
		{
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)0);
			glEnableVertexAttribArray(index + 1);
			glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)sizeof(glm::vec4));
			glEnableVertexAttribArray(index + 2);
			glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(index + 3);
			glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(3 * sizeof(glm::vec4)));
			
			glVertexAttribDivisor(index, divisor);
			glVertexAttribDivisor(index + 1, divisor);
			glVertexAttribDivisor(index + 2, divisor);
			glVertexAttribDivisor(index + 3, divisor);
		}

		uint32_t GetVerticesSize() const { return m_VerticesSize; }

		void Delete() const { glDeleteBuffers(1, &m_VBO); }
	};

}


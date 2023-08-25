#pragma once

#include "pch.h"

class UBO
{
private:
	uint32_t m_UBO;
public:
	UBO(uint32_t size)
	{
		glGenBuffers(1, &m_UBO);
		
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		Unbind();
	}

	void Bind() const { glBindBuffer(GL_UNIFORM_BUFFER, m_UBO); }
	void Unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

	void BindBufferRange(uint32_t index, uint32_t range, uint32_t offset) const
	{
		Bind(); 
		glBindBufferRange(GL_UNIFORM_BUFFER, index, m_UBO, offset, range); 
		Unbind();
	}

	void BufferSubData(uint32_t index, uint32_t size, uint32_t offset, const void* data) const
	{
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		Unbind();
	}
};
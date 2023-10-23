#pragma once

#include "pch.h"

#include "Shader.h"

namespace feng {

	class DepthMap
	{
	private:
		uint32_t m_DepthMapFBO;
		uint32_t m_DepthMapTexture;

		uint32_t m_TexWidth;
		uint32_t m_TexHeight;

		uint32_t m_QuadVAO;
		uint32_t m_QuadVBO;
		float m_QuadVertices[20];
	public:
		DepthMap(uint32_t texWidth, uint32_t texHeight)
			: m_TexWidth(texWidth), m_TexHeight(texHeight), m_QuadVertices {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f }
		{
			glEnable(GL_DEPTH_TEST);

			glGenVertexArrays(1, &m_QuadVAO);
			glGenBuffers(1, &m_QuadVBO);
			glBindVertexArray(m_QuadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
			glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &m_QuadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));

			glGenFramebuffers(1, &m_DepthMapFBO);

			glGenTextures(1, &m_DepthMapTexture);
			glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_TexWidth, m_TexHeight, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float bColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);

			glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapTexture, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
		}

		void BindTex(uint32_t texture_unit = 0) const
		{
			glActiveTexture(GL_TEXTURE0 + texture_unit);
			glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Draw(Shader& shader, uint32_t texture_unit = 0)
		{
			shader.Bind();
			glActiveTexture(GL_TEXTURE0 + texture_unit);
			glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
			glBindVertexArray(m_QuadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		uint32_t GetTexWidth() const { return m_TexWidth; }
		uint32_t GetTexHeight() const { return m_TexHeight; }
	};

}
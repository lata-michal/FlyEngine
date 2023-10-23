#pragma once

#include "pch.h"

#include "Shader.h"

namespace feng {

	class DepthMapEx
	{
	private:
		uint32_t m_DepthMapFBO;
		uint32_t m_DepthCubeMapTex;

		uint32_t m_TexWidth;
		uint32_t m_TexHeight;
	public:
		DepthMapEx(uint32_t texWidth, uint32_t texHeight)
			: m_TexWidth(texWidth), m_TexHeight(texHeight)
		{
			glGenFramebuffers(1, &m_DepthMapFBO);

			glGenTextures(1, &m_DepthCubeMapTex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubeMapTex);

			for (uint32_t i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_TexWidth, m_TexHeight,
					0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthCubeMapTex, 0);
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
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubeMapTex);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		uint32_t GetTexWidth() const { return m_TexWidth; } const
		uint32_t GetTexHeight() const { return m_TexHeight; } const
		float GetAspectRatio() const { return static_cast<float>(GetTexWidth()) / static_cast<float>(GetTexHeight()); } 
	};
}


#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Window.h"
#include "Log.h"

namespace feng {

	class FrameBuffer
	{
	private:
		VAO m_VAO;
		uint32_t m_IndicesCount;

		uint32_t m_MultiSampleFrameBuffer;
		uint32_t m_MultiSampleFrameBufferTexture;
		uint32_t m_RenderBuffer;

		uint32_t m_FrameBuffer;
		uint32_t m_TexColorBuffer;

		glm::vec3 m_SceneColor;

		float m_SceneExposure;
	public:
		FrameBuffer(const VBO& vbo, const IBO& ibo, VertexLayout& layout)
			: m_VAO(), m_IndicesCount(ibo.GetIndicesCount()), m_SceneColor(0.0f), m_SceneExposure(1.0f)
		{
			m_VAO.Setup(vbo, ibo, layout);

			if (Window::QueryAntiAliasing())
			{
				glGenFramebuffers(1, &m_MultiSampleFrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, m_MultiSampleFrameBuffer);

				glGenTextures(1, &m_MultiSampleFrameBufferTexture);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleFrameBufferTexture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Window::QueryAntiAliasing(), GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleFrameBufferTexture, 0);

				glGenRenderbuffers(1, &m_RenderBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, Window::QueryAntiAliasing(), GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					Log::Print("ERROR: FRAMEBUFFER IS NOT COMPLETE!");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glGenFramebuffers(1, &m_FrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

				glGenTextures(1, &m_TexColorBuffer);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					Log::Print("ERROR: FRAMEBUFFER IS NOT COMPLETE!");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else
			{
				glGenFramebuffers(1, &m_FrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

				glGenTextures(1, &m_TexColorBuffer);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0);

				glGenRenderbuffers(1, &m_RenderBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					Log::Print("ERROR: FRAMEBUFFER IS NOT COMPLETE!");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		void Draw(const Shader& postProcessing)
		{
			if (Window::QueryAntiAliasing())
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultiSampleFrameBuffer);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer);
				glBlitFramebuffer(0, 0, Window::GetWidth(), Window::GetHeight(), 0, 0, Window::GetWidth(), Window::GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

				Unbind();
				postProcessing.Bind();
				m_VAO.Bind();

				Window::DisableDepthTesting();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);

				Window::EnableDepthTesting();
			}
			else
			{
				Unbind();
				postProcessing.Bind();
				m_VAO.Bind();

				Window::DisableDepthTesting();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);

				Window::EnableDepthTesting();
			}
		}

		void Update()
		{
			if (Window::QueryAntiAliasing())
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_MultiSampleFrameBuffer);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleFrameBufferTexture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Window::QueryAntiAliasing(), GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MultiSampleFrameBufferTexture, 0);

				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, Window::QueryAntiAliasing(), GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0);

				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
			}
		}

		void Bind()  
		{ 
			if (Window::QueryAntiAliasing())
				glBindFramebuffer(GL_FRAMEBUFFER, m_MultiSampleFrameBuffer); 
			else
				glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		}

		float GetExposure(float exposureFactor, float minimumExposure, float maximumExposure, float exposureAdjustmentSpeed)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGetTexImage(GL_TEXTURE_2D, static_cast<uint32_t>(log2(static_cast<float>(max(Window::GetWidth(), Window::GetHeight())))), GL_RGB, GL_FLOAT, &m_SceneColor);

			float averageTone = m_SceneColor.r * 0.2126f + m_SceneColor.g * 0.7152f + m_SceneColor.b * 0.0722f;

			m_SceneExposure = glm::mix(m_SceneExposure, 1.0f / (averageTone * exposureFactor), exposureAdjustmentSpeed);
			m_SceneExposure = glm::clamp(m_SceneExposure, minimumExposure, maximumExposure);

			return m_SceneExposure;
		}

		void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		
		void Delete() { glDeleteFramebuffers(1, &m_FrameBuffer); glDeleteRenderbuffers(1, &m_RenderBuffer); }
	};

}
#include <BloomEffect.h>

#include "Log.h"

using namespace feng;

feng::BloomEffect::BloomEffect(const std::string& upscaleShaderPath, const std::string& downscaleShaderPath, float bloomStrength, 
	float filterRadius, float applyBloomBrightnessThreshold, uint32_t nBloomMips)
	: m_QuadVertices({1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }), m_QuadIndices({ 0, 1, 2, 0, 2, 3 }),
	m_UpscaleShader(upscaleShaderPath), m_DownscaleShader(downscaleShaderPath), m_BloomStrength(bloomStrength), m_FilterRadius(filterRadius), 
	m_ApplyBloomBrightnessThreshold(applyBloomBrightnessThreshold), m_NumBloomMips(nBloomMips), m_Quad()
{
	m_QuadLayout.AddVec2(0);
	m_QuadLayout.AddVec2(1);
	m_Quad = Mesh(m_QuadVertices, m_QuadIndices, m_QuadLayout);

	glGenFramebuffers(1, &m_FrameBufferSrc);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferSrc);

	glGenTextures(1, &m_FBOTextureSrc);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTextureSrc);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTextureSrc, 0);

	glGenRenderbuffers(1, &m_RenderBufferSrc);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferSrc);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferSrc);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Log::Print("ERROR: FRAMEBUFFER IS NOT COMPLETE!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &m_FrameBufferBloom);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBloom);

	glm::ivec2 tmpMipSize(Window::GetWidth(), Window::GetHeight());

	for (uint32_t i = 0; i < m_NumBloomMips; i++)
	{
		bloomMip mip;

		tmpMipSize /= 2;
		mip.size = tmpMipSize;

		glGenTextures(1, &mip.bloomTexID);
		glBindTexture(GL_TEXTURE_2D, mip.bloomTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, mip.size.x, mip.size.y, 0, GL_RGB, GL_HALF_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_BloomMips.emplace_back(mip);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomMips[0].bloomTexID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Log::Print("ERROR: FRAMEBUFFER IS NOT COMPLETE!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_DownscaleShader.Bind();
	m_DownscaleShader.SetUniform1i("uSourceTex", 0);

	m_UpscaleShader.Bind();
	m_UpscaleShader.SetUniform1i("uSourceTex", 0);
}

feng::BloomEffect::~BloomEffect()
{
	glDeleteFramebuffers(1, &m_FrameBufferSrc); 
	glDeleteFramebuffers(1, &m_FrameBufferBloom);
}

void feng::BloomEffect::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferSrc);
	glBindTexture(GL_TEXTURE_2D, m_FBOTextureSrc);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Window::GetWidth(), Window::GetHeight(), 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTextureSrc, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferSrc);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::GetWidth(), Window::GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferSrc);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBloom);

	glm::ivec2 tmpMipSize(Window::GetWidth(), Window::GetHeight());

	for (uint32_t i = 0; i < m_NumBloomMips; i++)
	{
		bloomMip mip;

		tmpMipSize /= 2;
		mip.size = tmpMipSize;

		glGenTextures(1, &mip.bloomTexID);
		glBindTexture(GL_TEXTURE_2D, mip.bloomTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, mip.size.x, mip.size.y, 0, GL_RGB, GL_HALF_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_BloomMips[i] = mip;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomMips[0].bloomTexID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void feng::BloomEffect::BindSourceFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferSrc);
}

void feng::BloomEffect::UnbindSourceFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void feng::BloomEffect::RenderSceneWBloom(Shader& shader) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBloom);

	Window::DisableDepthTesting();

	RenderDownsamples();
	RenderUpsamples();

	Window::EnableDepthTesting();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Window::SetViewport(0, 0, Window::GetWidth(), Window::GetHeight());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTextureSrc);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_BloomMips[0].bloomTexID);

	shader.Bind();
	shader.SetUniform1i("uSceneTex", 0);
	shader.SetUniform1i("uBloomTex", 1);
	shader.SetUniform1f("uBloomStrength", m_BloomStrength);

	Window::DisableDepthTesting();
	m_Quad.Draw(shader);
	Window::EnableDepthTesting();
}

void feng::BloomEffect::RenderDownsamples()
{
	m_DownscaleShader.Bind();
	m_DownscaleShader.SetUniform2f("uSourceResolution", static_cast<float>(Window::GetWidth()), static_cast<float>(Window::GetHeight()));
	m_DownscaleShader.SetUniform1i("uFirstIteration", 1);
	m_DownscaleShader.SetUniform1f("uApplyBloomThreshold", m_ApplyBloomBrightnessThreshold);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTextureSrc);

	for (auto& mip : m_BloomMips)
	{
		Window::SetViewport(0, 0, mip.size.x, mip.size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mip.bloomTexID, 0);

		m_Quad.Draw(m_DownscaleShader);

		m_DownscaleShader.SetUniform2f("uSourceResolution", static_cast<float>(mip.size.x), static_cast<float>(mip.size.y));
		m_DownscaleShader.SetUniform1i("uFirstIteration", 0);
		glBindTexture(GL_TEXTURE_2D, mip.bloomTexID);
	}

	m_DownscaleShader.Unbind();
}

void feng::BloomEffect::RenderUpsamples()
{
	m_UpscaleShader.Bind();
	m_UpscaleShader.SetUniform1f("uFilterRadius", m_FilterRadius);

	Window::EnableBlending();
	Window::SetBlendFunc(GL_ONE, GL_ONE);
	Window::SetBlendEquation(GL_FUNC_ADD);

	for (uint32_t i = m_BloomMips.size() - 1; i > 0; i--)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BloomMips[i].bloomTexID);

		Window::SetViewport(0, 0, m_BloomMips[i - 1].size.x, m_BloomMips[i - 1].size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomMips[i - 1].bloomTexID, 0);

		m_Quad.Draw(m_UpscaleShader);
	}

	Window::DisableBlending();
	m_UpscaleShader.Unbind();
}

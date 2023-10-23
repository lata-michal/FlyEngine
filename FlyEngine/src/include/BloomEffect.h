#pragma once

#include "pch.h"

#include "Shader.h"
#include "Window.h"
#include "Mesh.h"

namespace feng {

	struct bloomMip
	{
		glm::ivec2 size;
		uint32_t bloomTexID;
	};

	class BloomEffect
	{
	private:
		Shader m_UpscaleShader;
		Shader m_DownscaleShader;

		Mesh m_Quad;
		VertexLayout m_QuadLayout;
		std::vector<float> m_QuadVertices;
		std::vector<uint32_t> m_QuadIndices;

		uint32_t m_FrameBufferSrc;
		uint32_t m_RenderBufferSrc;
		uint32_t m_FBOTextureSrc;

		uint32_t m_FrameBufferBloom;
		std::vector<bloomMip> m_BloomMips;

		float m_FilterRadius;
		float m_ApplyBloomBrightnessThreshold;
		uint32_t m_NumBloomMips;
	public:
		BloomEffect(const std::string& upscaleShaderName, const std::string& downscaleShaderName,
			float filterRadius, float applyBloomBrightnessThreshold = 1.0f, uint32_t nBloomMips = 5);
		~BloomEffect();

		void BindSourceFBO() const;
		void UnbindSourceFBO() const;

		void RenderSceneWBloom(Shader& shader);

		void Update();
	private:
		void RenderDownsamples();
		void RenderUpsamples();
	};

}
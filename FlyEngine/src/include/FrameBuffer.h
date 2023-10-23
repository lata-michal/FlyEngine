#pragma once

#include "pch.h"

#include "Shader.h"
#include "VertexArray.h"
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
		FrameBuffer(const VBO& vbo, const IBO& ibo, VertexLayout& layout);
		~FrameBuffer();

		void Draw(const Shader& postProcessing);

		void Update();

		void Bind() const;
		void Unbind() const;

		float GetExposure(float exposureFactor, float minimumExposure, float maximumExposure, float exposureAdjustmentSpeed);
	};

}
#pragma once

#include "pch.h"

namespace feng {

	class Texture
	{
	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Type;
		unsigned char* m_LocalBuffer;
		int32_t m_Width, m_Height, m_NrChannels;
	public:
		Texture(const std::string& path, const std::string& type, int32_t wrap_param, bool flip = true);

		void Bind(uint32_t texture_unit = 0) const;
		void Unbind() const;

		void DeleteTexture();

		const std::string& GetType() const { return m_Type; }
		const std::string& GetPath() const { return m_FilePath; }

		inline int32_t GetWidth() const { return m_Width; }
		inline int32_t GetHeight() const { return m_Height; }
	};

}
#include "Texture.h"

#include "Log.h"

using namespace feng;

Texture::Texture(const std::string& path, const std::string& type, int32_t wrap_param, bool flip)
	: m_Type(type), m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_NrChannels(0)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(flip);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

	int32_t external_format = -1;
	int32_t internal_format = GL_RGBA8;

	if (m_NrChannels == 1)
	{
		external_format = GL_RED;
	}
	else if (m_NrChannels == 3)
	{
		if(type != "texture_specular" && type != "texture_normal" && type != "texture_displacement")
			internal_format = GL_SRGB;

		external_format = GL_RGB;
	}
	else if (m_NrChannels == 4)
	{
		if(type != "texture_specular" && type != "texture_normal" && type != "texture_displacement")
			internal_format = GL_SRGB_ALPHA;

		external_format = GL_RGBA;
	}

	if (m_LocalBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_Width, m_Height, 0, external_format, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		Log::Print("FAILED TO LOAD TEXTURE! \n");
	}
	stbi_image_free(m_LocalBuffer);
}

void Texture::Bind(uint32_t texture_unit) const
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::DeleteTexture()
{
	glDeleteTextures(1, &m_RendererID);
}

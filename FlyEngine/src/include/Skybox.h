#pragma once

#include "VertexArray.h"
#include "Shader.h"

namespace feng {

    class Skybox
    {
    private:
        uint32_t m_TextureID;
        VAO m_VAO;
        VBO m_VBO;
    public:
        Skybox(const std::vector<std::string>& faces)
            : m_VAO(), m_VBO({ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
                      -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
                       1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
                      -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
                      -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
                      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f })
        {
            VertexLayout skyboxVerticesLayout;
            skyboxVerticesLayout.AddVec3(0); 
            m_VAO.Setup(m_VBO, skyboxVerticesLayout);
            
            loadCubeMap(faces);
        }

        void Draw(Shader& shader)
        {
            shader.Bind();
            m_VAO.Bind();

            glDepthFunc(GL_LEQUAL);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_TextureID);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glDepthFunc(GL_LESS);
        }

        uint32_t GetTexture() const { return m_TextureID; }
    private:
        void loadCubeMap(const std::vector<std::string>& faces)
        {
            glGenTextures(1, &m_TextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

            int32_t width, height, nrChannels;

            stbi_set_flip_vertically_on_load(false);

            for (uint32_t i = 0; i < faces.size(); i++)
            {
                unsigned char* buffer = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                if (buffer)
                {
                    uint32_t colorChannels = 0;
                    uint32_t externalColorChannels = 0;

                    if (nrChannels == 3)
                    {
                        colorChannels = GL_SRGB;
                        externalColorChannels = GL_RGB;
                    }
                    else if (nrChannels == 4)
                    {
                        colorChannels = GL_SRGB_ALPHA;
                        externalColorChannels = GL_RGBA;
                    }

                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorChannels, width, height, 0, externalColorChannels, GL_UNSIGNED_BYTE, buffer);
                    stbi_image_free(buffer);
                }
                else
                {
                    Log::Print("FAILED TO LOAD CUBE MAP TEXTURE AT PATH: " + faces[i] + '\n');
                    stbi_image_free(buffer);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
    };

}
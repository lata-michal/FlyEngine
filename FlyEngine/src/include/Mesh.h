#pragma once

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace feng {

    class Mesh
    {
    private:
        VAO m_VAO;
        VBO m_VBO;
        IBO m_IBO;
        std::vector<Texture> m_Textures;
    public:
        Mesh(const std::vector<float>& vertices, VertexLayout& layout);
        Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, VertexLayout& layout);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures, const std::vector<float>& instanceMatrix = {});
        Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures, VertexLayout& layout);

        void Draw(Shader& shader, uint32_t instanceCount = 1);

        inline uint32_t GetVAO() const { return m_VAO.GetVAO(); }
    };

}
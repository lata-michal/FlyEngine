#include "Mesh.h"

using namespace feng;

Mesh::Mesh(const std::vector<float>& vertices, VertexLayout& layout)
    : m_VAO(), m_VBO(vertices), m_IBO()
{
    m_VAO.Setup(m_VBO, layout);
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<Texture>& textures, VertexLayout& layout)
    : m_VAO(), m_VBO(vertices), m_IBO(), m_Textures(textures)
{
    m_VAO.Setup(m_VBO, layout);
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, VertexLayout& layout)
    : m_VAO(), m_VBO(vertices), m_IBO(indices)
{
    m_VAO.Setup(m_VBO, m_IBO, layout);
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures, VertexLayout& layout)
    : m_VAO(), m_VBO(vertices), m_IBO(indices), m_Textures(textures)
{
    m_VAO.Setup(m_VBO, m_IBO, layout);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures, const std::vector<float>& instanceMatrix)
    : m_VAO(), m_VBO(vertices), m_IBO(indices), m_Textures(textures)
{
    VertexLayout layout;
    layout.AddVec3(0);
    layout.AddVec3(1);
    layout.AddVec2(2);
    layout.AddVec3(3);
    layout.AddVec3(4);

    m_VAO.Setup(m_VBO, m_IBO, layout);

    if (!instanceMatrix.empty())
    {
        VBO instanceVBO(instanceMatrix);
        instanceVBO.SetupInstanceVBOMat4(m_VAO.GetVAO(), 3, 1);
    }
}

void Mesh::Draw(Shader& shader, uint32_t instanceCount)
{
    shader.Bind();

    uint32_t diffuseNr = 1;
    uint32_t specularNr = 1;
    uint32_t normalNr = 1;
    uint32_t displacementNr = 1;

    for (uint32_t textureUnit = 0; textureUnit < m_Textures.size(); textureUnit)
    {
        std::string number;
        std::string name = m_Textures[textureUnit].GetType();

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_displacement")
            number = std::to_string(displacementNr++);
    
        shader.SetUniform1i(("uMaterial." + name + number).c_str(), textureUnit);

        m_Textures[textureUnit].Bind(textureUnit++);
    }

    m_VAO.Bind(); 

    if(instanceCount == 1)
        if(m_IBO.GetIndicesCount())
            glDrawElements(GL_TRIANGLES, m_IBO.GetIndicesCount(), GL_UNSIGNED_INT, 0);
        else
           glDrawArrays(GL_TRIANGLES, 0, m_VAO.GetVerticesCount());
    else
        if (m_IBO.GetIndicesCount())
            glDrawElementsInstanced(GL_TRIANGLES, m_IBO.GetIndicesCount(), GL_UNSIGNED_INT, 0, instanceCount);
        else
            glDrawArraysInstanced(GL_TRIANGLES, 0, m_VAO.GetVerticesCount(), instanceCount);

    m_VAO.Unbind();
}
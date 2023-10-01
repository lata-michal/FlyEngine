#pragma once

#include "pch.h"

namespace feng {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        Vertex() = default;

        Vertex(float x, float y, float z, float xn, float yn, float zn, float u, float v, 
            float xtan, float ytan, float ztan, float xbtan, float ybtan, float zbtan)
            : position(x, y, z), normal(xn, yn, zn), texCoords(u, v), tangent(xtan, ytan, ztan), bitangent(xbtan, ybtan, zbtan) { }

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec3 tangent, glm::vec3 bitangent)
        {
            this->position = position;
            this->normal = normal;
            this->texCoords = texCoords;
            this->tangent = tangent;
            this->bitangent = bitangent;
        }
    };
    
    struct VertexLayout {
        std::map<uint32_t, uint32_t> attribSize;
        uint32_t stride;

        VertexLayout() : stride(0) { };

        inline void AddVec2(uint32_t position)
        {
            attribSize[position] = 2;
            stride += (attribSize[position] * sizeof(float));
        }

        inline void AddVec3(uint32_t position)
        {
            attribSize[position] = 3;
            stride += (attribSize[position] * sizeof(float));
        }
        
        uint32_t GetOffset(uint32_t position) 
        {
            uint32_t offset = 0;

            for (uint32_t i = 0; i < position; i++)
                offset += (attribSize[i]);

            offset *= sizeof(float);
            return offset;
        }
    };
}
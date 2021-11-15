#pragma once
#include <array>
#include <glad/glad.h>

namespace CG {
    struct VBO {
        unsigned int id;
        VBO() {
            glGenBuffers(1, &this->id);
        }
        ~VBO() {
            glDeleteBuffers(1, &this->id);
        }

        template<std::size_t size>
        void copyToBuffer(const std::array<float, size> vertices) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
        }
        
        void bind() {
            glBindBuffer(GL_ARRAY_BUFFER, this->id);
        }
    };
}

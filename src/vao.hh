#pragma once
#include <functional>
#include <glad/glad.h>

namespace CG {
    struct VAO {
        unsigned int id;
        VAO(std::function<void ()> setup) {
            glGenVertexArrays(1, &this->id);
            use(setup);
        }
        ~VAO() {
            glDeleteVertexArrays(1, &this->id);
        }
        void use(std::function<void ()> usage) {
            glBindVertexArray(this->id);
            usage();
        }
    };
}

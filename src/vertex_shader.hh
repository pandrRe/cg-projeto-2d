#pragma once
#include "./file_reader.hh"
#include "glad/glad.h"

namespace CG {
    struct VertexShader {
        unsigned int id;
        VertexShader(const char* path) {
            auto vertex_shader_src = read_file(path);
            this->id = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(this->id, 1, (const GLchar**)&vertex_shader_src, NULL);
            glCompileShader(this->id);
            this->check_compilation();
        }
        ~VertexShader() {
            glDeleteShader(this->id);
        }

        void check_compilation() {
            int success;
            char info_log[512];
            glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(this->id, 512, NULL, info_log);
                std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
            }
        }
    };
}

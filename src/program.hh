#pragma once
#include "./vertex_shader.hh"
#include "./fragment_shader.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

//O Program é nossa entidade principal nesse projeto.
//Ele é responsável por carregar os shaders e guardar um estado de transformação.
namespace CG {
    struct Program {
        unsigned int id;
        VertexShader vertex_shader;
        FragmentShader fragment_shader;
        glm::mat4 transform_matrix;
        float rotation = 0.0f;
        float scale = 1.0f;
        float translation_x = 0.0f;
        float translation_y = 0.0f;

        Program(const char* vertex_shader_path, const char* fragment_shader_path)
            : vertex_shader(VertexShader(vertex_shader_path)),
            fragment_shader(FragmentShader(fragment_shader_path))
        {
            this->id = glCreateProgram();
            this->transform_matrix = glm::mat4(1.0f);
            glAttachShader(this->id, vertex_shader.id);
            glAttachShader(this->id, fragment_shader.id);
            glLinkProgram(this->id);
            this->checkLinkingSuccess();
        }

        ~Program() {
            glDeleteProgram(this->id);
        }

        void checkLinkingSuccess() {
            int success;
            char info_log[512];
            glGetProgramiv(this->id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(this->id, 512, NULL, info_log);
                std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
            }
        }

        float radians() {
            return this->rotation * 2 * glm::pi<float>();
        }

        //As transformações são recriadas a cada renderização.
        void applyTransformations() {
            this->transform_matrix = glm::mat4(1.0f);
            this->transform_matrix = glm::translate(this->transform_matrix, glm::vec3(this->translation_x, this->translation_y, 0.0f));
            this->transform_matrix = glm::rotate(this->transform_matrix, this->radians(), glm::vec3(0.0, 0.0, 1.0));
            this->transform_matrix = glm::scale(this->transform_matrix, glm::vec3(this->scale, this->scale, this->scale));
            this->setMat4("transform", this->transform_matrix);
        }

        void use() {
            glUseProgram(this->id);
            this->applyTransformations();
        }

        void setBool(const std::string &name, bool value) const {         
            glUniform1i(glGetUniformLocation(this->id, name.c_str()), value); 
        }

        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(this->id, name.c_str()), value); 
        }

        void setFloat(const std::string &name, float value) const { 
            glUniform1f(glGetUniformLocation(this->id, name.c_str()), value); 
        }

        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        void rotate(float value) {
            this->rotation += value;
            if (this->rotation > 1.0f) {
                this->rotation -= 1.0f;
            }
            else if (this->rotation < 0.0f) {
                this->rotation = 1.0f + this->rotation;
            }
        }

        void change_scale(float value) {
            if (this->scale + value < 0.5f) {
                this->scale = 0.5f;
            }
            else if (this->scale + value > 2.0f) {
                this->scale = 2.0f;
            }
            else {
                this->scale += value;
            }
        }

        void translate_x(float value) {
            if (this->translation_x + value < -1.0f) {
                this->translation_x = -1.0f;
            }
            else if (this->translation_x + value > 1.0f) {
                this->translation_x = 1.0f;
            }
            else {
                this->translation_x += value;
            }
        }

        void translate_y(float value) {
            if (this->translation_y + value < -1.0f) {
                this->translation_y = -1.0f;
            }
            else if (this->translation_y + value > 1.0f) {
                this->translation_y = 1.0f;
            }
            else {
                this->translation_y += value;
            }
        }

        //O Program lida com o input do usuário.
        void handle_key_press(int key) {
            switch(key) {
                case GLFW_KEY_Q:
                    this->rotate(0.01f);
                    break;
                case GLFW_KEY_W:
                    this->rotate(-0.01f);
                    break;
                case GLFW_KEY_A:
                    this->change_scale(-0.05f);
                    break;
                case GLFW_KEY_S:
                    this->change_scale(0.05f);
                    break;
                case GLFW_KEY_UP:
                    this->translate_y(0.05f);
                    break;
                case GLFW_KEY_DOWN:
                    this->translate_y(-0.05f);
                    break;
                case GLFW_KEY_RIGHT:
                    this->translate_x(0.05f);
                    break;
                case GLFW_KEY_LEFT:
                    this->translate_x(-0.05f);
                    break;
            }
        }
    };
}

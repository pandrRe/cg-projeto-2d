#include <glad/glad.h>
#include "./src/vbo.hh"
#include "./src/vao.hh"
#include "./src/program.hh"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <array>

//Caminhos para os shaders.
constexpr auto BASE_VERTEX_SHADER_PATH { "/home/pandrre/projects/GLFW-CMake-starter/shaders/base_vertex_shader.glsl" };
constexpr auto BASE_FRAGMENT_SHADER_PATH { "/home/pandrre/projects/GLFW-CMake-starter/shaders/base_fragment_shader.glsl" };

//Dimensões da tela.
constexpr int RENDER_WIDTH { 1024 };
constexpr int RENDER_HEIGHT { 768 };

//Vértices normalizados do triângulo a ser renderizado.
constexpr std::array<float, 9> TRIANGLE_VERTICES {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

//Função passada para o GLFW reportar erros.
void report_error(int code, const char* message) {
    std::cout << "ERROR -- CODE: " << code << " -- " << message << std::endl;
}

//Função passada para o GLFW atualizar a área de renderização de acordo com o tamanho da janela.
void update_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwSetErrorCallback(report_error); //definindo o callback de erros.
    GLFWwindow* window;

    //Inicialização do GLFW.
    if (!glfwInit())
        return -1;

    //Indicando versão e perfil do OpenGL.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Inicializando uma janela OpenGL.
    window = glfwCreateWindow(RENDER_WIDTH, RENDER_HEIGHT, "Projeto 2D - Computação Gráfica", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, update_viewport); //callback para redimensionamento da janela.

    //Colocando a janela aberta como contexto atual.
    glfwMakeContextCurrent(window);
    gladLoadGL(); //Carregando a biblioteca auxiliar glad.
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f ); //Definindo a cor de fundo.

    //Inicialização das entidades principais.
    //Mais explicações sobre a função de cada uma dentro de cada classe.
    auto base_program = CG::Program(BASE_VERTEX_SHADER_PATH, BASE_FRAGMENT_SHADER_PATH);
    auto vbo = CG::VBO();
    auto vao = CG::VAO([&vbo] {
        vbo.bind();
        vbo.copyToBuffer(TRIANGLE_VERTICES); //Vértices copiados para o buffer.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Indicando como o buffer deve ser lido.
        glEnableVertexAttribArray(0);
    });

    glfwSetWindowUserPointer(window, &base_program); //Guardando um ponteiro para o nosso Program dentro da janela.
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        //Assim, é possível acessar o programa de dentro desse callback.
        auto& program = *static_cast<CG::Program*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            program.handle_key_press(key); //Ações de teclado são processadas pelo Program.
        }
    });

    //Loop de renderização.
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); //Limpeza da tela.
        base_program.use(); //Ativando o programa no contexto atual.
        vao.use([] { //Ao usar um VAO, repito as ações que foram definidas em sua criação.
            glDrawArrays(GL_TRIANGLES, 0, 3); //Indico para o OpenGL desenhar o que está no buffer.
        });

        //Troca de buffers.
        glfwSwapBuffers(window);

        //Checagem de eventos.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

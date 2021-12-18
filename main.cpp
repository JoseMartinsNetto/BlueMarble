#include <iostream>
#include <cassert>
#include <array>

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

const int Width = 800;
const int Height = 600;

int main() {

	// Inicializa GLFW
	assert(glfwInit() == GLFW_TRUE);

	// Criar janela
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	assert(Window);

	// Ativa o contexto na janela
	glfwMakeContextCurrent(Window);

	// Inicial OpenGL e GLEW
	assert(glewInit() == GLEW_OK);

	// Verificar a versao do OpenGL que estamos usando
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGl Version: " << GLMajorVersion << "." << GLMinorVersion << std::endl;
	std::cout << "OpenGl Version: " << GLMajorVersion << "." << GLMinorVersion << std::endl;
	std::cout << "OpenGl Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGl Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Definir um triangulo em cordenada normalizada
	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{-1.0f, -1.0f, 0.0f },
		glm::vec3{ 1.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f }
	};

	// Copiar os vertices do triangulo para a memoria da GPU
	// Pedir para o OpenGl gerar o identificador do vertexbuffer
	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);

	// Ativer o VertexBuffer como o buffer para onde ser'a copiado os dados do triangulo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	// copiar os dados para a mem'oria de v'ideo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);

	// Definir cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Loop de eventos
	while (!glfwWindowShouldClose(Window)) {

		// Limpar o frameBuffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		// Ativar o vertexBuffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// desenhar o triangulo que estao no vertexBuffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Desativar os atributos antes ativados
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);

		// Processa todos os eventos na fila
		glfwPollEvents();

		// Envia o conteudo do framebuffer da janela para ser desenhada
		glfwSwapBuffers(Window);
	}

	// Desalocar o vertexbuffer
	glDeleteBuffers(1, &VertexBuffer);

	// Finaliza GLFW
	glfwTerminate();

	return 0;
}
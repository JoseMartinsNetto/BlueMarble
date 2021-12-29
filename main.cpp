#include <iostream>
#include <cassert>
#include <array>

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

const int Width = 800;
const int Height = 600;

void LogGLInfo() {
	std::cout << "OpenGl Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGl Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGl Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);

		char* message = (char*)alloca(lenght * sizeof(char));

		glGetShaderInfoLog(id, lenght, &lenght, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main() {

	assert(glfwInit() == GLFW_TRUE);

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	assert(Window);

	glfwMakeContextCurrent(Window);

	assert(glewInit() == GLEW_OK);

	LogGLInfo();

	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{-0.5f, -0.5f, 0.0f },
		glm::vec3{ 0.5f, -0.5f, 0.0f },
		glm::vec3{ 0.0f, 0.5f, 0.0f }
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"void main()\n"
		"{\n"
		"	gl_position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);

	glUseProgram(shader);

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(Window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();

		glfwSwapBuffers(Window);
	}

	glDeleteBuffers(1, &buffer);

	glfwTerminate();

	return 0;
}
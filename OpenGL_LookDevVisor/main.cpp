#include <iostream>
#include <Windows.h>
#define  GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include "Shaders_Manager.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


GLfloat Vertices[] = {	 0.5f,  0.5f, 0.0f,  // Top Right
						 0.5f, -0.5f, 0.0f,  // Bottom Right
						-0.5f, -0.5f, 0.0f,  // Bottom Left
						-0.5f,  0.5f, 0.0f   // Top Left 
};

GLuint indices[] = {
	0, 1, 3,   // First Triangle
	1, 2, 3    // Second Triangle
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LookDev Visor", nullptr, nullptr);
	if (window == nullptr){
		std::runtime_error("Imposible to create the window");
		glfwTerminate();
		return -1;
	};
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		std::runtime_error("Can't not possible init GLEW");
		glfwTerminate;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//Shader Loading and binding
	char* vertexShaderFile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\vertexshader.glsl";
	char* fragmentshaderfile = "C:\\Users\\Lauren\\Documents\\Visual Studio 2017\\Projects\\LookDev_VisorRT\\OpenGL_LookDevVisor\\Shaders\\fragmentshader.glsl";
		
	Shaders_Manager ShaderManager;
	bool shaderesult;
	GLuint myshader;
	myshader = ShaderManager.loadShader(vertexShaderFile, fragmentshaderfile, &shaderesult);
	if (shaderesult) {
		std::cout << "Vertex shader and Fragment Shader Load: OK" << std::endl;
	};

	//Creating buffer for Vertex loading
	GLuint VAO,EBO, vertexBufferOb;

	//Init Vertex Array, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertexBufferOb);
	glGenBuffers(1, &EBO);

	//Creating the pipeline
	//1. Bind Vtx Array
	glBindVertexArray(VAO);
	//2.-
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferOb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	//3.-
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Binding Buffer and Shader inputs.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// APP LOOP

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//rendering commands
		glUseProgram(myshader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	};

	glfwTerminate;

	return 0;
};

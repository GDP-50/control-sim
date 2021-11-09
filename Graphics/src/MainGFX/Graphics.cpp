//
//  Graphics.hpp
//  sim
//
//  Created by Rufus Vijayaratnam on 9/11/2021.
//  Copyright Â© 2021 Rufus Vijayaratnam. All rights reserved.
//

#include "Graphics.hpp"


bool gfx::InitialiseGLFW() {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

GLFWwindow* gfx::OpenWindow(const char * windowName, bool &windowOpened) {
    GLFWwindow* window;
    window = glfwCreateWindow(720, 720, windowName, NULL, NULL);
    std::cout << "here window is: " << window << std::endl;

    if (window == NULL) {
        printf("Could not open a window, terminating... \n");
        //getchar();
        glfwTerminate();
        windowOpened = false;
        return window;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        printf("It is not ok... \n");
        getchar();
        glfwTerminate();
        windowOpened = false;
        return window;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    //glfwSetCursorPos(window, 1024 / 2, 720 / 2);

    //Set coloured background, Disable if some fancier method desired.
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    windowOpened = true;
    return window;
}

void gfx::Main(GLFWwindow* window) {

    GLuint golferVertexArrayID;
	glGenVertexArrays(1, &golferVertexArrayID);
	glBindVertexArray(golferVertexArrayID);

    GLuint caddyVertexArrayID;
	glGenVertexArrays(1, &caddyVertexArrayID);
	glBindVertexArray(caddyVertexArrayID);

    const char* vertexPath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Graphics/src/Shaders/SimpleVertexShader.vertexshader";
    const char* fragmentPath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Graphics/src/Shaders/SimpleFragmentShader.fragmentshader";

    GLuint programID = LoadShaders(vertexPath, fragmentPath);
    //GLuint programID = LoadShaders(vertexPath, fragmentPath);

	
    const int numCoords = 9 * circleRes;
    GLfloat* golfer_vertex_buffer_data = (GLfloat*)malloc(numCoords * sizeof(GLfloat));
    gfx::circleVertices(golfer_vertex_buffer_data);

    GLfloat* caddy_vertex_buffer_data = (GLfloat*)malloc(numCoords * sizeof(GLfloat));
    gfx::circleVertices(caddy_vertex_buffer_data);

    


	GLuint golfer_vertexbuffer;
	glGenBuffers(1, &golfer_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, golfer_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(golfer_vertex_buffer_data) * numCoords, golfer_vertex_buffer_data, GL_STATIC_DRAW);
	GLuint caddy_vertexbuffer;
	glGenBuffers(1, &caddy_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, caddy_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(caddy_vertex_buffer_data) * numCoords, caddy_vertex_buffer_data, GL_STATIC_DRAW);

    // Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 ViewMatrix = glm::lookAt(
				glm::vec3(0, 0, -3),           // Camera is here
				glm::vec3(0, 0, 0), // and looks here : at the same position, plus "direction"
				glm::vec3(0, 1, 0)           // Head is up (set to 0,-1,0 to look upside-down)
						   );

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);

    glm::mat4 caddyInitialTranslation = glm::mat4(
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.5, 0.5, 0.0, 0.0
        );
    
	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

        calculateTranslation(window);
        glm::mat4 translationMatrix = getTranslationMatrix();
        glm::mat4 golferModelMatrix = translationMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
		glm::mat4 MVP = golferModelMatrix * ProjectionMatrix * ViewMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, golfer_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, numCoords); 
		glDisableVertexAttribArray(0);


        //MARK: FOR CADDY
        glm::mat4 caddyModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.05, 0.05, 0.1));
		MVP = caddyModelMatrix * ProjectionMatrix * ViewMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, caddy_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, numCoords); 
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
    glDeleteBuffers(1, &golfer_vertexbuffer);
	glDeleteVertexArrays(1, &golferVertexArrayID);
	glDeleteBuffers(1, &caddy_vertexbuffer);
	glDeleteVertexArrays(1, &caddyVertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

}

void gfx::circleVertices(GLfloat* vertexData) {
    double angleStep = 2.0 * M_PI / circleRes;

    for (int i = 0; i < circleRes; i++) {
        double x1 = sin(i * angleStep);
        double y1 = cos(i * angleStep);
        double x2 = sin((i +1) * angleStep);
        double y2 = cos((i +1) * angleStep);
        *(vertexData + 9 * i) = 0.0;
        *(vertexData + 9 * i + 1) = 0.0;
        *(vertexData + 9 * i + 2) = 0.0;

        *(vertexData + 9 * i + 3) = x1;
        *(vertexData + 9 * i + 4) = y1;
        *(vertexData + 9 * i + 5) = 0.0;

        *(vertexData + 9 * i + 6) = x2;
        *(vertexData + 9 * i + 7) = y2;
        *(vertexData + 9 * i + 8) = 0.0;
    }
}


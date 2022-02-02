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

    GLuint redVao;
    GLuint greenVao;
	glGenVertexArrays(1, &redVao);
	glGenVertexArrays(1, &greenVao);

    const char* vertexPath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Graphics/src/Shaders/SimpleVertexShader.vertexshader";
    const char* redFragmentPath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Graphics/src/Shaders/SimpleFragmentShader.fragmentshader";
    const char* greenFragmentPath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Graphics/src/Shaders/GreenFragmentShader.fragmentshader";

    GLuint programID = LoadShaders(vertexPath, redFragmentPath);
    GLuint greenProgramID = LoadShaders(vertexPath, greenFragmentPath);

	
    const int numCoords = 9 * circleRes;
    GLfloat* golfer_vertex_buffer_data = (GLfloat*)malloc(numCoords * sizeof(GLfloat));
    gfx::circleVertices(golfer_vertex_buffer_data);

    GLfloat caddy_vertex_buffer_data[] = { 
		-1.0, -1.0, 0.0,
		 1.0, -1.0, 0.0,
		 0.0,  1.0, 0.0,
	};

    

    const int redItems = 2;
	GLuint redVertexBuffer[redItems];
    int redNumCoords[] = {numCoords, 9};
	glGenBuffers(2, redVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numCoords, golfer_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, caddy_vertex_buffer_data, GL_STATIC_DRAW);

    //PARSE THE COURSE
    const char* coursePath = "/mnt/c/Users/Rufus Vijayaratnam/Documents/University/GDP/control-sim/Coordinates/Hole 1.txt";
    int greenSize = 0;
    int bunkerCount = 0;
    int bunkerSizes[128];
    GLfloat tee[3];
    loadCourse(coursePath, greenSize, bunkerCount, bunkerSizes);

    GLfloat** green = (GLfloat**)malloc(greenSize * sizeof(GLfloat*));
    if (!green) {
        fprintf(stderr, "Failed to allocate memory for green ptr, exiting...\n");
    }
    for (int i = 0; i < greenSize; i++) {
        exit;
        green[i] = (GLfloat*)calloc(3, sizeof(GLfloat));
        if (!green[i]) {
            fprintf(stderr, "Could not allocate memory for green multi-dim array, exiting...\n");
            exit(1);
        }
    }
    
    GLfloat*** bunkers = (GLfloat***)malloc(bunkerCount * sizeof(GLfloat**));
    printf("Allocated for %d bunkers\n", bunkerCount);
    if (!bunkers) {
        fprintf(stderr, "Could not allocate memory for bunker multi-dim array, exiting...\n");
        exit;
    }
    for (int i = 0; i < bunkerCount; i++) {
        bunkers[i] = (GLfloat**)malloc(3 * bunkerSizes[i] * sizeof(GLfloat*));
        if (!bunkers[i]) {
            fprintf(stderr, "Could not allocate memory for bunker multi-dim array, part 2, exiting...\n");
            exit;
        }
        for (int j = 0; j < bunkerSizes[i]; j++) {
            bunkers[i][j] = (GLfloat*)calloc(3, sizeof(GLfloat));
            if (!bunkers[i]) {
                fprintf(stderr, "Could not allocate memory for bunker multi-dim array, part 3, exiting...\n");
                exit;
            }
        }
    }

    prepareCourse(coursePath, green, greenSize, bunkers, bunkerCount, bunkerSizes, tee);

    GLfloat* greenVertexData;
    greenVertexData = (GLfloat*)malloc(greenSize * 9 * sizeof(GLfloat));
    gfx::preparePolygonVertices(green, greenVertexData, greenSize);

    FILE* greenTest = fopen("../../greenTestM.txt", "w");
    for(int i = 0; i < greenSize * 9; i++){
        fprintf(greenTest, "%f\n", greenVertexData[i]);
    }
    fclose(greenTest);


    //Green Buffer
    GLuint greenBuffer;
    glGenBuffers(1, &greenBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, greenBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * greenSize * 9, greenVertexData, GL_STATIC_DRAW);

    //Bunker Buffers
    /* GLuint* bunkerBuffer = (GLuint*)malloc(bunkerCount * sizeof(GLuint));
    glGenBuffers(bunkerCount, bunkerBuffer);
    for(int i = 0; i < bunkerCount; i++) {
        int numBufferCoords = bunkerSizes[i] * 3;
        glBindBuffer(GL_ARRAY_BUFFER, bunkerBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, sizeof(GLfloat) * numBufferCoords, bunkers[i], GL_STATIC_DRAW);
    } */

    /* 
    GLuint redVertexBuffer[redItems];
    int redNumCoords[] = {numCoords, 9};
	glGenBuffers(2, redVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numCoords, golfer_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, caddy_vertex_buffer_data, GL_STATIC_DRAW);
     */

    // Get a handle for our "MVP" uniform
	GLuint redMatrixID = glGetUniformLocation(programID, "MVP");
	GLuint greenMatrixID = glGetUniformLocation(greenProgramID, "MVP");

    glm::mat4 ViewMatrix = glm::lookAt(
				glm::vec3(0, 0, -3),           // Camera is here
				glm::vec3(0, 0, 0), // and looks here : at the same position, plus "direction"
				glm::vec3(0, 1, 0)           // Head is up (set to 0,-1,0 to look upside-down)
						   );


    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);

    glm::vec3 caddyInitialPosition = glm::vec3(0, -0.5, 0);
    setCaddyTranslationMatrix(caddyInitialPosition);
    glm::mat4 caddyScaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.05, 0.05, 0.05));
    setCaddyRotationMatrix(0);

	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

        calculateTranslation(window);
        glm::mat4 translationMatrix = getTranslationMatrix();
        glm::mat4 golferModelMatrix = translationMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
		glm::mat4 MVP = golferModelMatrix * ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(redMatrixID, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(redVao);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[0]);
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


        glm::mat4 caddyTranslationMatrix = getCaddyTranslationMatrix();
        glm::mat4 caddyRotationMatrix = getCaddyRotationMatrix();
        glm::mat4 caddyModelMatrix = caddyTranslationMatrix * caddyRotationMatrix * caddyScaleMatrix;
		MVP = caddyModelMatrix * ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(redMatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, redVertexBuffer[1]);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 3); 
		glDisableVertexAttribArray(0);

        //BELOW FOR STATIC GOLF COURSE STUFF
        //For Green
        glUseProgram(greenProgramID);
        MVP = ProjectionMatrix * ViewMatrix;
        glUniformMatrix4fv(greenMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glBindVertexArray(greenVao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, greenBuffer);
        glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, greenSize * 9); 
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteProgram(programID);
	glDeleteProgram(greenProgramID);
    glDeleteBuffers(1, &redVertexBuffer[0]);
	glDeleteBuffers(1, &redVertexBuffer[1]);
	glDeleteVertexArrays(1, &redVao);
	glDeleteVertexArrays(1, &greenVao);
    glDeleteBuffers(1, &greenBuffer);

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

void gfx::preparePolygonVertices(GLfloat** prevObj, GLfloat* newObj, int n) {
    //First calculate centroid (cx, cy) cz = 0.0
    double A, cxa, cya, cx, cy;
    double xi, yi, xip1, yip1;
    A = 0;
    cxa = 0;
    cya = 0;
    for(int i = 0; i < n - 1; i++) {
        xi = prevObj[i][0];
        yi = prevObj[i][1];
        xip1 = prevObj[i + 1][0];
        yip1 = prevObj[i + 1][1];
        /* https://math.stackexchange.com/questions/3177/why-doesnt-a-simple-mean-give-the-position-of-a-centroid-in-a-polygon */
        A += 0.5 * (xi * yip1 - xip1 * yi);
        cxa += (xi + xip1) * (xi*yip1 - xip1*yi);
        cya += (yi + yip1) * (xi*yip1 - xip1*yi);
    }
    cx = cxa / (6.0 * A);
    cy = cya / (6.0 * A);


    for(int i = 0; i < n - 1; i++) {
        double x1, y1, x2, y2;
        x1 = prevObj[i][0];
        y1 = prevObj[i][1];
        x2 = prevObj[i + 1][0];
        y2 = prevObj[i + 1][1];

        newObj[9 * i + 0] = cx;
        newObj[9 * i + 1] = cy;
        newObj[9 * i + 2] = 0.0;

        newObj[9 * i + 3] = x1;
        newObj[9 * i + 4] = y1;
        newObj[9 * i + 5] = 0.0;

        newObj[9 * i + 6] = x2;
        newObj[9 * i + 7] = y2;
        newObj[9 * i + 8] = 0.0;
    }
}


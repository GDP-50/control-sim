//
//  Controls.hpp
//  sim
//
//  Created by Rufus Vijayaratnam on 9/11/2021.
//  Copyright Â© 2021 Rufus Vijayaratnam. All rights reserved.
//

#include "controls.hpp"
glm::mat4 translationMatrix;


/* glm::mat4 getProjectionMatrix() {
    return projectionMatrix;
}
glm::mat4 getViewMatrix() {
    return viewMatrix;
} */
glm::mat4 getTranslationMatrix() {
    return translationMatrix;
}

//glm::vec3 position = glm::vec3(0,0,0); //Centered
double speed = 5;
double tX, tY;

void calculateTranslation(GLFWwindow* window) {
    static double prevTime = glfwGetTime();
    double deltaTime = glfwGetTime() - prevTime;

    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		tY += deltaTime * speed;
	}
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		tY -= deltaTime * speed;
	}
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		tX -= deltaTime * speed;
	}
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		tX += deltaTime * speed;
	}

    translationMatrix = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        tX, tY, 0, 1
        );

    prevTime = glfwGetTime();
}
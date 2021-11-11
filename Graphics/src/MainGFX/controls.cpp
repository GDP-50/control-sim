//
//  Controls.hpp
//  sim
//
//  Created by Rufus Vijayaratnam on 9/11/2021.
//  Copyright Â© 2021 Rufus Vijayaratnam. All rights reserved.
//

#include "controls.hpp"
glm::mat4 translationMatrix;
glm::mat4 caddyTranslationMatrix;
glm::mat4 caddyRotationMatrix;
glm::mat3 caddyRotationMatrix3;
glm::vec3 caddyDirection = glm::vec3(0, 1, 0);
double caddyRotation = 0;


glm::mat4 getTranslationMatrix() {
    return translationMatrix;
}

glm::mat4 getCaddyTranslationMatrix() {
    return caddyTranslationMatrix;
}

glm::mat4 getCaddyRotationMatrix() {
    return caddyRotationMatrix;
}

void setCaddyInitialPosition(glm::mat4 &initialPos) {
    caddyTranslationMatrix = initialPos;
}

double speed = 2;
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

void caddyControl() {
    double pxg = translationMatrix[3][0];
    double pyg = translationMatrix[3][1];
    double pxc = caddyTranslationMatrix[3][0];
    double pyc = caddyTranslationMatrix[3][1];
    glm::vec3 golferPos = glm::vec3(pxg, pyg, 0);
    glm::vec3 caddyPos = glm::vec3(pxc, pyc, 0);
    glm::vec3 golferPosUnit = glm::normalize(golferPos);
    glm::vec3 chordVec = golferPos - caddyPos;
    double angle = angleBetweenVectors(caddyDirection, chordVec);
    caddyRotation = angle;
    setCaddyRotationMatrix(caddyRotation);
}

void setCaddyRotationMatrix(double theta) {
    caddyRotationMatrix3 = glm::mat3(
        cos(theta), -sin(theta), 0,
        sin(theta), cos(theta), 0,
                         0, 0, 1
    );

    caddyRotationMatrix = glm::mat4(
        cos(theta), -sin(theta), 0, 0,
        sin(theta), cos(theta), 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1
    );
}

double angleBetweenVectors(glm::vec3 &u, glm::vec3 &v) {
    double dot = glm::dot(u, v);
    double det = u[0] * v[1] - u[1] * v[0];
    double angle = atan2(dot, det) - M_PI / 2; //For some reason atan2 is off by half pi
    return angle;
}

double vec3Magnitude(glm::vec3 &u) {
    double sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum += pow((double)u[i], 2);
    }
    return sqrt(sum);
}

void printVec3(glm::vec3 &vec) {
    for (int i = 0; i < 3; i++) {
        printf("%0.2f ", vec[i]);
    }
    printf("\n");
}
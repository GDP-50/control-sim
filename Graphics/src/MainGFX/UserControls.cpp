#include "UserControls.hpp"

double xpos, ypos;
glm::vec3 caddyPos;
GLfloat scaleVal;


void clickCallback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        xpos /= ((double)width) / 2;
        ypos /= ((double)height) / 2;
        xpos -= 1;
        ypos -= 1; ypos *= -1;
        printf("xpos: %f, ypos: %f\n", xpos, ypos);
        calculateDistance();
    }
}

void calculateDistance() {
    glm::vec3 chord = glm::vec3((double)caddyPos[0] - xpos, (double)caddyPos[1] - ypos, 0.0);
    printf("chord is (%f, %f)\n", chord.x, chord.y);
    GLfloat mag = glm::length(chord);
    GLfloat meters = mag * (0.5 * scaleVal);
    printf("Distance is %f meters\n", meters);
}

void setCaddyPos(glm::vec3 pos) {
    caddyPos = pos;
    //printf("caddy pos set as (%f, %f)\n", caddyPos.x, caddyPos.y);
}

void setScaleVal(GLfloat val) {
    scaleVal = val;
}

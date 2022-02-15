//
//  Graphics.hpp
//  sim
//
//  Created by Rufus Vijayaratnam on 9/11/2021.
//  Copyright Â© 2021 Rufus Vijayaratnam. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp
#define circleRes 100
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <iostream>
#include <Shaders/shader.hpp>
#include <stdlib.h>
#include "controls.hpp"
#include "parseCourse.hpp"


namespace gfx {
    bool InitialiseGLFW();
    GLFWwindow* OpenWindow(const char * windowName, bool &windowOpened);
    void Main(GLFWwindow* window);
    void circleVertices(GLfloat* vertexData);
    void centrePolygon(GLfloat** polygon, int n, GLfloat pos[2]);
    void preparePolygonVertices(GLfloat** prevObj, GLfloat* newObj, int n);
}
#endif
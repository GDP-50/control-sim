//
//  Controls.hpp
//  sim
//
//  Created by Rufus Vijayaratnam on 9/11/2021.
//  Copyright Â© 2021 Rufus Vijayaratnam. All rights reserved.
//

#ifndef Controls_hpp
#define Controls_hpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include <stdio.h>
glm::mat4 getTranslationMatrix();
void calculateTranslation(GLFWwindow* window);
glm::mat4 getCaddyTranslationMatrix();
void caddyControl();
void setCaddyInitialPosition(glm::mat4 &initialPos);
glm::mat4 getCaddyRotationMatrix();
double angleBetweenVectors(glm::vec3 &u, glm::vec3 &v);
double vec3Magnitude(glm::vec3 &u);
void setCaddyRotationMatrix(double theta);
void printVec3(glm::vec3 &vec);
#endif
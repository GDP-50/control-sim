#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef UserControls_hpp
#define UserControls_hpp
void clickCallback(GLFWwindow* window, int button, int action, int mods);
void calculateDistance();
void setCaddyPos(glm::vec3 pos);
void setScaleVal(GLfloat val);
#endif
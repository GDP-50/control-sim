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
glm::mat4 getTranslationMatrix();
void calculateTranslation(GLFWwindow* window);
#endif
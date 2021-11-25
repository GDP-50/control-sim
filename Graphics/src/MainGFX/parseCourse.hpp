#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include <GL/glew.h>

#ifndef ParseCourse_hpp
#define ParseCourse_hpp
void loadCourse(const char * path, GLfloat** green, int &greenSize, GLfloat*** bunkers, int &bunkerCount, int bunkerSizes[128], GLfloat tee[3]);
void prepareCourse(GLfloat** green, int greenSize, GLfloat*** bunkers, int bunkerCount, int bunkerSizes[128], GLfloat teePos[3]);
#endif
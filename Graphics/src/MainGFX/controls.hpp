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
#include <sys/time.h>
#include "Globals.hpp"
#include <limits.h>
glm::mat4 getTranslationMatrix();
void calculateTranslation(GLFWwindow* window);
glm::mat4 getCaddyTranslationMatrix();
void caddyControl();
glm::mat4 getCaddyRotationMatrix();
double angleBetweenVectors(glm::vec3 &u, glm::vec3 &v);
double vec3Magnitude(glm::vec3 &u);
void setCaddyRotationMatrix(double theta);
void printVec3(glm::vec3 &vec);
void updateCaddy();
void setCaddyTranslationMatrix(glm::vec3 &t);
bool inPolygon(glm::vec3 pos, GLfloat** polygon, int n);
void rayCast(GLfloat** polygon, int* intersections, int idxI, int idxIp1,GLfloat px, GLfloat py);
glm::mat4 makeTranslationMatrix(GLfloat tx, GLfloat ty);
void minMax(GLfloat* v1, GLfloat* v2, GLfloat* min, GLfloat* max);
void Max(GLfloat* v1, GLfloat* v2, GLfloat* max);
void Min(GLfloat* v1, GLfloat* v2, GLfloat* min);
void pathfind(GLfloat gx, GLfloat gy, GLfloat cx, GLfloat cy, GLfloat path[3][2]);
bool vecPolygonIntersect(glm::vec3 point1, glm::vec3 point2, GLfloat** polygon, int n);
bool segmentIntersection(glm::vec3 point1, glm::vec3 point2, int idxI, int IdxIp1, GLfloat** polygon);
void pathFind(glm::vec3 caddyPos, glm::vec3 targetPos, int polyIdx);
void initTime();
void updateTime();
void nearestPolygon(glm::vec3 caddyPos, int* polyIdx);
#endif
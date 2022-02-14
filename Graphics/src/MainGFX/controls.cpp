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
double caddySpeed = 0.02;

bool timerShouldReset = false;


glm::mat4 getTranslationMatrix() {
    return translationMatrix;
}

glm::mat4 getCaddyTranslationMatrix() {
    return caddyTranslationMatrix;
}

glm::mat4 getCaddyRotationMatrix() {
    return caddyRotationMatrix;
}


void setCaddyTranslationMatrix(glm::vec3 &t) {
    double tx = t[0];
    double ty = t[1];

    caddyTranslationMatrix = glm::mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            tx, ty, 0, 1
        );
}

double speed = 1;
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

    updateCaddy();
}

void caddyControl() {
    static double prevTime = glfwGetTime();
    if (timerShouldReset)  {
        timerShouldReset = false;
        prevTime = glfwGetTime();
    }
    double deltaTime = glfwGetTime() - prevTime;
    double pxg = translationMatrix[3][0];
    double pyg = translationMatrix[3][1];
    double pxc = caddyTranslationMatrix[3][0];
    double pyc = caddyTranslationMatrix[3][1];
    glm::vec3 golferPos = glm::vec3(pxg, pyg, 0);
    glm::vec3 caddyPos = glm::vec3(pxc, pyc, 0);
    
    glm::vec3 chordVec = glm::normalize(golferPos - caddyPos);
    caddyPos += (float)(deltaTime * speed) * chordVec;
    setCaddyTranslationMatrix(caddyPos);

    double angle = angleBetweenVectors(caddyDirection, chordVec);
    caddyRotation = angle;
    setCaddyRotationMatrix(caddyRotation);
    prevTime = glfwGetTime();
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

void updateCaddy() {
    double pxg = translationMatrix[3][0];
    double pyg = translationMatrix[3][1];
    double pxc = caddyTranslationMatrix[3][0];
    double pyc = caddyTranslationMatrix[3][1];
    glm::vec3 golferPos = glm::vec3(pxg, pyg, 0);
    glm::vec3 caddyPos = glm::vec3(pxc, pyc, 0);

    glm::vec3 chordVec = golferPos - caddyPos;
    int scale = 100;

    if (vec3Magnitude(chordVec) <= 0.5) {
        timerShouldReset = true;
        return;
    }
    caddyControl();

}

glm::mat4 makeTranslationMatrix(GLfloat tx, GLfloat ty) {
    glm::mat4 tm = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        tx, ty, 0, 1
        );
    return tm;
}

bool rayCast(GLfloat** polygon, int m, glm::vec3 pos) {
    GLfloat vx, vy, mag, maxXp, maxYp, minXp, minYp, px, py, eps;
    int intersections = 0;
    GLfloat** vecs = (GLfloat**)malloc(m * sizeof(GLfloat*));
    GLfloat* maxX = (GLfloat*)malloc(m * sizeof(GLfloat));
    GLfloat* maxY = (GLfloat*)malloc(m * sizeof(GLfloat));
    GLfloat* minX = (GLfloat*)malloc(m * sizeof(GLfloat));
    GLfloat* minY = (GLfloat*)malloc(m * sizeof(GLfloat));
    if(!vecs) {
        printf("Could not allocate for vecs in rayCast\n");
        exit(1);
    }
    for(int i = 0; i < m; i++) {
        vecs[i] = (GLfloat*)malloc(2 * sizeof(GLfloat));
        if(!vecs[i]) {
        printf("Could not allocate for vecs[%d] in rayCast\n", i);
        exit(1);
        }
    }
    for(int i = 0; i < m - 1; i++) {
        maxXp = polygon[i + 1][0];
        minXp = polygon[i][0];
        maxYp = polygon[i + 1][1];
        minYp = polygon[i][0];
        vx = abs(maxXp - minXp);
        vy = abs(maxYp - minYp);
        mag = glm::sqrt(glm::pow(vx, 2.0) + glm::pow(vy, 2.0));
        vx /= mag;
        vy /= mag;
        vecs[i][0] = vx;
        vecs[i][1] = vy;

        maxX[i] = (maxXp >= minXp) ? maxXp : minXp;
        minX[i] = (minXp < maxXp) ? minXp : maxXp;
        maxY[i] = (maxYp >= minYp) ? maxYp : minYp;
        minY[i] = (minYp < maxYp) ? minYp : maxYp;
    }
    px = pos.x;
    py = pos.y;
    eps = 0.01;
    
    for(int i = 0; i < m; i++) {
        py = (py == polygon[i][1]) ? (py + eps) : py;
        if(py >= minY[i] && py <= maxY[i]) {
            if(px <= maxX[i]) {
                intersections++;
            }
        }
    }

    /* Free arrays */
    for(int i = 0; i < m; i++) {
        free(vecs[i]);
    }
    free(vecs);
    free(maxX);
    free(maxY);
    free(minX);
    free(minY);

    if(intersections % 2 == 0) {
        return false;
    } else {
        return true;
    }
}
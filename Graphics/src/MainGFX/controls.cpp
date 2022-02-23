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
glm::vec3 target;
bool activeTarget = false;
glm::vec3 yUnit = glm::vec3(0, 1, 0);
glm::vec3 zUnit = glm::vec3(0, 0, 1);
double caddyRotation = 0;
double caddySpeed = 0.02;

bool timerShouldReset = false;
long double initialTime_s;
long double time_s;

double xIncrement = -1;
double yIncrement = -1;

int nearestPolyIdx;

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
    nearestPolygon(caddyPos, &nearestPolyIdx);

/*     double pxc = caddyTranslationMatrix[3][0];
    double pyc = caddyTranslationMatrix[3][1];
    glm::vec3 caddyPos = glm::vec3(pxc, pyc, 0);
    if(!activeTarget) {
        pathFind(caddyPos, golferPos, )
    }
 */
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

bool inPolygon(glm::vec3 pos, GLfloat** polygon, int n) {
    GLfloat px, py;
    px = pos.x;
    py = pos.y;
    int intersections;
    intersections = 0;
    for(int i = 0; i < n - 1; i++) {
        rayCast(polygon, &intersections, i, i + 1, px, py);
    }
    rayCast(polygon, &intersections, n - 1, 0, px, py);
    if(intersections % 2 == 0) {
        return false;
    } else {
        return true;
    }
}

void rayCast(GLfloat** polygon, int* intersections, int idxI, int idxIp1, GLfloat px, GLfloat py) {
    GLfloat x1, x2, y1, y2, xMin, xMax, yMin, yMax;
    x1 = polygon[idxI][0];
    y1 = polygon[idxI][1];
    x2 = polygon[idxIp1][0];
    y2 = polygon[idxIp1][1];
    minMax(&x1, &x2, &xMin, &xMax);
    minMax(&y1, &y2, &yMin, &yMax);
    if(px <= xMax) {
        if(py == yMin || py == yMax) {
            py += 0.001;
        }
        if(py > yMin && py < yMax) {
            (*intersections) += 1;
        }
    }
}

void minMax(GLfloat* v1, GLfloat* v2, GLfloat* min, GLfloat* max) {
    GLfloat temp;
    if(*v1 <= *v2) {
        *max = *v2;
        *min = *v1;
    } else {
        temp = *v2;
        *max = *v1;
        *min = temp;
    }
}
void Max(GLfloat* v1, GLfloat* v2, GLfloat* max) {
    *max = (*v1 <= *v2) ? *v2 : *v1; 
}
void Min(GLfloat* v1, GLfloat* v2, GLfloat* min) {
    *min = (*v1 <= *v2) ? *v1 : *v2; 
}

bool vecPolygonIntersect(glm::vec3 point1, glm::vec3 point2, GLfloat** polygon, int n) {
    for(int i = 0; i < n - 1; i++) {
        if(segmentIntersection(point1, point2, i, i + 1, polygon)) return true;
    }
    return false;
}

bool segmentIntersection(glm::vec3 point1, glm::vec3 point2, int idxI, int idxIp1, GLfloat** polygon) {
    /* Vector a is caddy to golfer, vector b is polygon edge */
    /* x and y are the intersection point of the line segments */
    GLfloat x, y, ax1, ax2, ay1, ay2, bx1, bx2, by1, by2;
    GLfloat grad_a, grad_b;
    GLfloat ac, bc;
    GLfloat minX, maxX, minY, maxY;
    ax1 = point2.x; ay1 = point2.y;
    ax2 = point1.x; ay2 = point1.y;
    bx1 = polygon[idxI][0]; by1 = polygon[idxI][1];
    bx2 = polygon[idxIp1][0]; by2 = polygon[idxIp1][1];

    /* First check that segments aren't parallel */
    grad_a = (ay2 - ay1) / (ax2 - ax1);
    grad_b = (by2 - by1) / (bx2 - bx1);
    //printf("grad a: %f, grad b: %f\n", grad_a, grad_b);
    ac = ay1 - grad_a * ax1;
    bc = by1 - grad_b * bx1;
    if(isinf(abs(grad_a)) && isinf(abs(grad_b))) return false;
    if(grad_a == grad_b) {
        /* We also have to consider that they might be collienar, they are if c i.e y = mc + c is equal for both*/
        printf("were parallel\n");
        if(ac == bc)  {
            printf("collinear\n");
            return true;
        } else {
            return false;
        } 
    }
    /* printf("idxI: %d, idxIp1: %d\n", idxI, idxIp1);
    if(isnan(grad_b)) {
        printf("was nan\n");
        printf("by2: %f, by1: %f, bx2: %f, bx1: %f\n", by2, by1, bx2, bx1);
    } */

    /* Expressions for x and y can be obtained by solving:
    Eq1: 0 = (ay2 - ay1) / (ax2 - ax1) * (x - ax1) + ay1 - y
    Eq2: 0 = (by2 - by1) / (bx2 - bx1) * (x - bx1) + by1 - y
    Eq1 and Eq2 can be derived from y = mx + c
    */
    x = (ax1*ay2*bx1 - ax1*ay2*bx2 - ax1*bx1*by2 + ax1*bx2*by1 - ax2*ay1*bx1 + ax2*ay1*bx2 + ax2*bx1*by2 - ax2*bx2*by1)/(ax1*by1 - ax1*by2 - ax2*by1 + ax2*by2 - ay1*bx1 + ay1*bx2 + ay2*bx1 - ay2*bx2);
    y = (ax1*ay2*by1 - ax1*ay2*by2 - ax2*ay1*by1 + ax2*ay1*by2 - ay1*bx1*by2 + ay1*bx2*by1 + ay2*bx1*by2 - ay2*bx2*by1)/(ax1*by1 - ax1*by2 - ax2*by1 + ax2*by2 - ay1*bx1 + ay1*bx2 + ay2*bx1 - ay2*bx2);
    /* Set valid intersection region
    This must be done after calculating x and y because a and b vec values are modified
    */
    /* sets smaller value to 1 index and larger value to 2 index*/
    minMax(&ax1, &ax2, &ax1, &ax2);
    minMax(&bx1, &bx2, &bx1, &bx2);
    minMax(&ay1, &ay2, &ay1, &ay2);
    minMax(&by1, &by2, &by1, &by2);
    /* Set intersectino region min/max x/y  */
    Max(&ax1, &bx1, &minX);//Min x = largest of ax1 and bx1
    Min(&ax2, &bx2, &maxX);//max x = smallest of ax2 and bx2
    Max(&ay1, &by1, &minY);
    Min(&ay2, &by2, &maxY);
    
    if(x >= minX && x <= maxX && y >= minY && y <= maxY) {
        printf("Valid intersection\n");
        return true;
    } else {
        return false;
    }
}

void pathFind(glm::vec3 caddyPos, glm::vec3 targetPos, int polyIdx) {
    GLfloat** polygon = polygons[polyIdx];
    int n = polyInfo[polyIdx][2];
    if(vecPolygonIntersect(targetPos, caddyPos, polygon, n)) {
        double yDisp, xDisp, theta;
        glm::vec3 chord, center;
        glm::vec3 point, rotatedPoint, rcaddyPos, rtargetPos;
        glm::mat3 rotationMatrix, unRotationMatrix;
        glm::vec3 a1, a2; 
        bool yShouldRepeat, xShouldRepeat;
        GLfloat** rotatedPoly;
        chord = targetPos - caddyPos;
        theta = angleBetweenVectors(chord, yUnit);
        rotationMatrix = glm::mat3(cos(theta), -sin(theta),  0,
                                    sin(theta), cos(theta),   0,
                                                        0, 0, 1);
        rotatedPoly = (GLfloat**)malloc(n * sizeof(GLfloat));
        /* Transform to new rotated coordinate system for ease */
        rtargetPos = targetPos * rotationMatrix;
        rcaddyPos = caddyPos * rotationMatrix;
        for(int i = 0; i < n; i++) {
            rotatedPoint = point * rotationMatrix;
            rotatedPoly[i] = (GLfloat*)malloc(3 * sizeof(GLfloat));
            rotatedPoly[i][0] = rotatedPoint.x;
            rotatedPoly[i][1] = rotatedPoint.y;
            rotatedPoly[i][2] = 0.0;
        }
        /* The segment points of horizontal line that we slide down */
        /* First for Y */
        yShouldRepeat = true;
        yDisp = 0.0;
        a1 = glm::vec3(rcaddyPos.x, rcaddyPos.y + yDisp, 0.0);
        a2 = glm::vec3(rtargetPos.x, rcaddyPos.y + yDisp, 0.0);
        while(yShouldRepeat) {
            while(vecPolygonIntersect(a1, a2, rotatedPoly, n)) {
                yDisp += yIncrement;
                a1 = glm::vec3(rcaddyPos.x, rcaddyPos.y + yDisp, 0.0);
                a2 = glm::vec3(rtargetPos.x, rcaddyPos.y + yDisp, 0.0);
                center = glm::vec3((a1.x + a2.x) / 2, (a1.y + a2.y) / 2, 0.0);
            }
            if(inPolygon(center, rotatedPoly, n)) {
                yIncrement *= -1;
                yDisp = 0;
                a1 = glm::vec3(rcaddyPos.x, rcaddyPos.y + yDisp, 0.0);
                a2 = glm::vec3(rtargetPos.x, rcaddyPos.y + yDisp, 0.0);
            } else {
                yShouldRepeat = false;
            }
        }
        xDisp = 0.0;
        a1 = glm::vec3(rcaddyPos.x + xDisp, rcaddyPos.y, 0.0);
        a2 = glm::vec3(rcaddyPos.x + xDisp, rcaddyPos.y + yDisp, 0.0);
        while(vecPolygonIntersect(a1, a2, rotatedPoly, n)) {
            xDisp += xIncrement;
            a1 = glm::vec3(rcaddyPos.x + xDisp, rcaddyPos.y, 0.0);
            a2 = glm::vec3(rcaddyPos.x + xDisp, rcaddyPos.y + yDisp, 0.0);
        }
        unRotationMatrix = glm::mat3(cos(-theta), -sin(-theta),  0,
                                    sin(-theta), cos(-theta),   0,
                                                         0, 0, 1);
        target = glm::vec3(xDisp + rcaddyPos.x, yDisp + rcaddyPos.y, 0.0) * unRotationMatrix;
    }
    target = targetPos;
}

void nearestPolygon(glm::vec3 caddyPos, int* polyIdx) {
    double cx, cy, cpx, cpy;
    double dist, smallestDist;
    int closestIdx;
    cpx = (double)caddyPos.x; cpy = (double)caddyPos.y;
    smallestDist = (double)LONG_MAX; /* max integer of other 64 bit type */
    for(int i = 0; i < polyCount; i++) {
        cx = polyInfo[i][0];
        cy = polyInfo[i][1];
        dist = sqrt(pow(cx - cpx, 2.0) + pow(cy - cpy, 2.0));
        if(dist < smallestDist) {
            closestIdx = i;
            smallestDist = dist;
        }
    }
    *polyIdx = closestIdx;
}

void initTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    initialTime_s = (long double)tv.tv_sec + (long double)(tv.tv_usec / 1000000.0);
}

void updateTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_s = (long double)tv.tv_sec + (long double)(tv.tv_usec / 1000000.0) - initialTime_s;
}
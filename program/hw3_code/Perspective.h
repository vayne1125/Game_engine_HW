#pragma once
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#define CAMERA_POS       10
#define   PI   3.1415927
using glm::vec3;
class Perspective{
public:
//double zNear = 0.1, zFar = 2000, aspect = WINDOW_WIDTH / (double)WINDOW_HEIGHT, fovy;
    Perspective(float aspect_);
    vec3 dir;
    vec3 pos;     //攝影機位置
    vec3 seePoint;  //看的位置
    float fovy;
    float zNear = 0.1,zFar = 2000;
    float aspect;
    float eyeAngY;
    virtual void update();
    void use();
    float eyeMtx[16];
    int mouseX,mouseY;
};
class FPPerspective:public Perspective{
public:
    bool first = 1;
    FPPerspective(float aspect_);
    void update() override;
    void keyEvent(unsigned char key);
    void passiveMotionEvent(int x,int y);
};

class TPPerspective:public Perspective{
public:
    void mouseWheelEvent(int button, int dir, int x, int y);
    void motionEvent(int x,int y);
    void mouseClickEvent(int btn, int state, int x, int y);
    TPPerspective(float aspect_);
    void update() override;
    float eyeDis = 0;             //
    int mouseBtn = 0;

};


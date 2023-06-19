#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <bits/stdc++.h>
#define ATTACK_SLIME 0
#define SLIME_DIE 1
#define ATTACK_ROBOT 2
#define LEVEL_UP 3
using std::list;
using std::string;
class UI{
public:
    void draw();
    UI(float asp);
    void addMsg(int type,int slimename);
    void addMsg(int type);
private:
    unsigned int head[1];
    float aspect;
    list<string> msg;
};
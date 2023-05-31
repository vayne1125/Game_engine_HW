#pragma once
#include<bits/stdc++.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#define DIR_LIGHT_POS    4
#define DIR_LIGHT_DIR    5
#define DIR_LIGHT_COLOR  6
#define DIR_LIGHT_EXPO   7
#define DIR_LIGHT_STR    8
#define CAMERA_POS       10
#define AMBIENT          11
#define SPOT_LIGHT_POS 13
#define SPOT_LIGHT_DIR 14
#define SPOT_LIGHT_COLOR 15
#define SPOT_LIGHT_EXPO 16
#define SPOT_LIGHT_STR 17
class Light
{
public:
    // type 0平行 1點光/spot light
    // pos x,y,z
    // dir x,y,z 射出去的方向
    // exop 0點光 1spot
    // str 
    Light(int type,float pos_x,float pos_y,float pos_z,float dir_x,float dir_y,float dir_z,float color_r,float color_g,float color_b,int expo,float str);
    
    void use();
    void use(float pos_x,float pos_y,float pos_z);
private:
    int type;
    float pos_x, pos_y,pos_z, dir_x, dir_y, dir_z,color_r,color_g,color_b;
    int expo;
    float str;
};

#pragma once
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include"GraphicObj.h"
#include"mytex.h"
#include "Billboard.h"
#include "Light.h"
#include "Robot.h"
#include "AISlime.h"
#define DETCOLLX 0
#define DETCOLLZ 1
#define SJ_ROBOT 2
class SceneJungle{
public:
    SceneJungle();
    void draw(float* eyeMtx,int programID);
    void useLight();
    bool detectCollision(float x,float z,int MODEL); 
private:
	Light* dirLight,* spotLight;    
    AISlime* fireslime;
};
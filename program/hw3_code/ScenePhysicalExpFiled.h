#pragma once
#include <vector>
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include"GraphicObj.h"
#include"mytex.h"
#include "Elf.h"
#include "Billboard.h"
class ScenePhysicalExpFiled {
public:
	//ScenePhysicalExpFiled();
	void draw(float* eyeMtx,int programID);
private:
	float floorX = 232;
	float floorY = 10;
	float floorZ = 203;
    float pos[3] = {0,0,213};    //左下角座標
};


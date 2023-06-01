#pragma once
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../glad/glad.h"
#include "GraphicObj.h"
#include "mytex.h"
#include "Object.h"
class ScenePhysicalExpFiled {
public:
	ScenePhysicalExpFiled();
	void draw(float* eyeMtx,int programID);
	void keyEvent(unsigned char key);
		Object* chooseObject;
		Object* object[3];
		int force = 50;
private:
	float pretime = 0;
	//int force = 50;
	int chooseID = 0;
	//Object* object[3];
	float floorX = 232;
	float floorY = 10;
	float floorZ = 203;
    float pos[3] = {0,0,213};    //左下角座標
};


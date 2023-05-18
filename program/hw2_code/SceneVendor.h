#pragma once
#include <vector>
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include"GraphicObj.h"
#include"mytex.h"
#include "Elf.h"
#include "Billboard.h"
class SceneVendor {
public:
	SceneVendor();
	void draw(float* eyeMtx,int programID);
	Elf* spotLightElf;
	float fountainPos[3] = { 116.0f,0.0f,124.0f }; 
	vector<vector<float>> cloud = { {100,40,96},{180,40,150},{55,40,170} };
	int eevee_ani = 0;
private:
	float floorX = 232;
	float floorY = 10;
	float floorZ = 203;
};

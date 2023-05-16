#pragma once
#include<GL/glew.h>
#include<GL/glut.h>
#include"../glad/glad.h"
#include"mesh.h"
#include"mytex.h"
class Billboard {
public:
	void draw(float x, float z, float w, float h, texture* tex,float* eyeMtx,unsigned int programID);
    //mesh* meshForBillboard;
private:
    //mesh* meshForBillboard;
};
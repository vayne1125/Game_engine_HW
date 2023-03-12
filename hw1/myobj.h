#pragma once
#include<iostream>
#include <vector>
#include <string>
#include <GL\freeglut.h>
#include"glad\glad.h"
#include"mesh.h"
using namespace std;
class myobj {
public:
	mesh* cube, * solidsphere, * cylinder, * solidtorus_5_025;
	myobj(unsigned int programID);
private:
	mesh* getMesh(string fname,unsigned int programID);
};

#pragma once
#include <vector>
#include<GL\freeglut.h>
#include"glad\glad.h"
using namespace std;
class mesh {
public:
	mesh(int programID,const vector<float>& vec);
	void draw(int programID);
private:
	unsigned int VAOID;
	int vertex_count = 0;
	

};

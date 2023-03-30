#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glad/glad.h"
#include "mesh.h"
using namespace std;
class myobj {
public:
	mesh* cube, * solidsphere, * cylinder, * solidtorus_5_025, *fountain, *blacksmith, * fountain_base , * fountain_water,* vendor_base, * vendor_col,* vendor_roof;
	mesh* square, *vendor_desk, *tree_round_up, *tree_round_btn,* tree_conical_btn,* tree_conical_up, *cloud1, *vendor_crate, * apple, * cheese, *bread2, *plate;
	myobj(unsigned int programID);																						  
private:																												  
	mesh* getMesh(string fname,unsigned int programID);
};

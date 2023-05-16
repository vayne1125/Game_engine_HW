#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include"myobj.h"
#include "mytex.h"
class magicwand {
public:
    magicwand(unsigned int programID);
    void draw(int programID);
private:
    bool show = 1;   //是否顯示
};
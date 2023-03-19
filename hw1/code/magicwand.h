#pragma once
#include<iostream>
#include <vector>
#include <string>
#include <GL\freeglut.h>
#include"glad\glad.h"
#include"myobj.h"
#include "mytex.h"
class magicwand {
public:
    magicwand(unsigned int programID);
    void draw(int programID);
private:
    bool show = 1;   //¬O§_Εγ₯ά
};
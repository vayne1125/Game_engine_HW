#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <bits/stdc++.h>
class UI{
public:
    void draw();
    UI(float asp){aspect = asp;}
private:
    float aspect;

};
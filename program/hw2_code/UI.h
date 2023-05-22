#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <bits/stdc++.h>
#include "Object.h"
class UI{
public:
    void draw();
    void draw(const Object& object,float pos[3], float dir[3],float F);
    UI(float asp){aspect = asp;vecs.resize(8);vecs2.resize(4);}
private:
    void changeInfo(const Object& object,float pos[3], float dir[3],float F);
    std::string fixed2(float val);
    float aspect;
    std::vector<std::string> vecs;
    std::vector<std::string> vecs2,vecs3;
    std::vector<std::string> modelForObject = {
        "--object info--",
        "name: ",
        "pos: ",
        "weight: "," kg",
        "velocity: "," m/s",
        "angular velocity: "," rad/s",
        "drag force (","), press Z to switch",
        "gravity (","), press X to switch"};
    std::vector<std::string> modelForBullet = {
        "--bullet info--",
        "pos: ",
        "dir: ",
        "force: "," N, press +/- to change"};
    std::vector<std::string> modelForOthers = {
        "--reset--",
        "press T/t to stop  (all)object",
        "press R/r to reset (all)object"}; 
};
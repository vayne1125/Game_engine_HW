#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <bits/stdc++.h>
class Object
{
    public:    
    std::string id = "cube1";
    glm::vec3 v{0.333, 0.333, 0.33};
    glm::vec3 w{0.22, 0.33, 0.33};
    glm::vec3 pos{1.009,22.111,3.22};
    glm::vec3 lin_a{1.009,22.111,3.22};
    glm::vec3 rot_a{1.009,22.111,3.22};
    float m = 100.0, k;
    bool isOpenDragForce = 0;
    bool isOpenGravity = 0;
};
class UI{
public:
    void draw(const Object& object,const std::vector<float>& pos,const std::vector<float>& dir,float F);
    void changeInfo(const Object& object,const std::vector<float>& pos,const std::vector<float>& dir,float F);
    std::string fixed2(float val);
    UI(float asp){aspect = asp;vecs.resize(8);vecs2.resize(4);}
private:
    float aspect;
    std::vector<std::string> vecs;
    std::vector<std::string> vecs2;
    std::vector<std::string> modelForObject = {
        "--object info--",
        "ID: ",
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
};
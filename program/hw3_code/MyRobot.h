#include "Robot.h"
using glm::vec3;

class MyRobot:public Robot{
public: 
    MyRobot(unsigned int programID,float pos_x,float pos_y,float pos_z);
    void keyEvent(unsigned char key);
    vec3 dir = {0,0,1};
    int blood = 200;
};
#include "Robot.h"
using glm::vec3;

class MyRobot:public Robot{
public: 
    MyRobot(unsigned int programID,float pos_x,float pos_y,float pos_z);
    void keyEvent(unsigned char key);
    vec3 dir = {0,0,1};
    int money = 100;
    int bulletAtk = 20;
    int level = 1;
    int exp = 7;
    void addBlood(float b){
        blood += b;
        if(b < 0) blood = fmax(blood,0);
        else blood = fmin(blood,MAX_BLOOD);
    };
    void addExp(int e);
    float getBlood(){return blood;};
private:
    float blood = 200;
    float MAX_BLOOD = 200.0;
};
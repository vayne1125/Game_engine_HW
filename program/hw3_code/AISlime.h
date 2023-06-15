#pragma once
#include "Object.h"
#include "math.h"
#include "Perspective.h"
#define FIRE 0
#define WATER 1
#define LIGHT 2
#define FEROCIOUS 0
#define TIMID 1
#define NORMAL 2
#define GOAWAY 1
#define MOVETOROBOT 2
using glm::vec3;
class AISlime{
public:
    AISlime(int textureID_,int AIID_,vec3 pos_,int sz_);
    vec3 pos;
    void FSM();
    void attack();
    void move_animation();
    void draw(unsigned int programID);
    bool isChoose(const glm::vec3& start, const glm::vec3& dir);
    void shoot(const glm::vec3& start, const glm::vec3& dir);
    float jumpYoffset = 0.3;  //跳多高
    float moveToRobotOffset = 0.067;
    float goAwayOffset = 0.2;
    float standByOffset = 0.1;
private:
    float getDis(float x1, float y1, float x2, float y2) {           //算距離
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    void fericious_fsm();
    void timid_fsm();
    void normal_fsm();
    void update();
    void standBy();
    void move(int TYPE);
    int sz = 1;
    int type = 0;
    int state = 0,moveAnimationState = 0,standByState = 0;
    vec3 standByDir{0,0,0};
    bool flag = 0;
    int AIID = 0,textureID = 0;
    float angY = 0;
};
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

#define STANDBY 0
#define RUNAWAY 1
#define MOVETOROBOT 2
#define ATTACK  3
#define QUICK_RUNAWAY 4
#define QUICK_MOVETOROBOT 5

using glm::vec3;

struct BulletInfo{
    vec3 pos,dir;
    int state = 0;
    BulletInfo(vec3 pos_,vec3 dir_):pos(pos_),dir(dir_){
        float m = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
        dir/=m; 
    };
};

//高1.75 寬2.5
class AISlime{
public:
    string name;
    AISlime(int textureID_,int AIID_,vec3 pos_,int sz_);
    vec3 pos;
    void FSM();
    void attack();
    void move_animation();
    void draw(unsigned int programID);
    bool isChoose();
    void shoot();
    float jumpYoffset = 0.3;  //跳多高
    float moveToRobotOffset = 0.2;
    float runAwayOffset = 0.3;
    float standByOffset = 0.1;
    float blood = 100,moveAnimationStateMax = 30;
    int sz = 1;
private:
    float getDis(float x1, float y1, float x2, float y2) {           //算距離
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    float getDis(vec3 a,vec3 b) {           //算距離
        return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]));
    }
    void fericious_fsm();
    void timid_fsm();
    void normal_fsm();
    void update();
    void standBy();
    void move();
    //int sz = 1;
    int type = 0;
    int state = STANDBY,moveAnimationState = 0,standByState = 0;
    vec3 standByDir{0,0,0};
    bool flag = 0;
    int AIID = 0,textureID = 0;
    float angY = 0;
    float attackAngY = 0; 
    bool injuried = 0;
    int attackRange = 0;
    int detectRange = 0;
    vector<BulletInfo> bullet;
    int bulletGenState = 0;
    //int bloodstate = 0;
};
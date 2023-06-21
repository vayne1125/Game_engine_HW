#pragma once
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include"../glad/glad.h"
#include"GraphicObj.h"
#include"mytex.h"
#include "Billboard.h"
#include "Light.h"
#include "Robot.h"
#include "AISlime.h"
#define DETCOLLX 0
#define DETCOLLZ 1
#define SJ_ROBOT 2

// struct BulletInfo{
//     vec3 pos,dir;
//     int state = 0;
//     BulletInfo(vec3 pos_,vec3 dir_):pos(pos_),dir(dir_){
//         float m = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
//         dir/=m; 
//     };
// };

class SceneJungle{
public:
    SceneJungle();
    void draw(float* eyeMtx,int programID);
    void useLight();
    bool detectCollision(float x,float z,int MODEL); 
    void mouseClickEvent(int btn, int state, int x, int y);
    void passiveMotionEvent(int x,int y);
    void keyEvent(unsigned int key);
    AISlime* chooseSlime;
    bool haveChooseSlime = 0;
private:
    float getDis(float x1, float y1, float x2, float y2) {           //算距離
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    float getDis(vec3 a,vec3 b) {           //算距離
        return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]));
    }
	Light* dirLight,* spotLight;    
    vector<AISlime>slime;
    vector<BulletInfo>bullet;
    void attackSlime();
    void addSlime();
    int slimeGenState = 0;
    int slimeTex[3] = {YU_SLIME_FIRE,YU_SLIME_LIGHT,YU_SLIME_WATER};
    int slimeAI[3] = {NORMAL,TIMID,FEROCIOUS};
    float fogColor[4] = { 1,1,1,0.1 };
};
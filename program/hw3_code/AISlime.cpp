#include "AISlime.h"
#include "MyRobot.h"
#include "SceneJungle.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
extern MyRobot *myRobot;
extern FPPerspective* fpperspective;
extern SceneJungle* sceneJungle;
AISlime::AISlime(int textureID_, int AIID_, vec3 pos_, int sz_)
{
    textureID = textureID_;
    AIID = AIID_;
    pos = pos_;
    sz = sz_;
}

void AISlime::FSM()
{
    if(AIID == FEROCIOUS) fericious_fsm();
    else if(AIID == TIMID) timid_fsm();
    else if(AIID == NORMAL) normal_fsm();
}

void AISlime::attack()
{

}

void AISlime::move_animation()
{
    if(!flag){
        pos[1]+= jumpYoffset;
        moveAnimationState++;
        if(moveAnimationState == 30) flag = 1;
    }else{
        pos[1]-= jumpYoffset;
        moveAnimationState--;
        if(moveAnimationState == 0) flag = 0;
    }
}

void AISlime::draw(unsigned int programID)
{
    FSM();
    glm::mat4 tp;
    //angY是弧度
    tp = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1),angY,glm::vec3(0.0,1.0,0.0)) * glm::scale(glm::mat4(1), {sz,sz,sz});
    glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);

    myTex->useByID(textureID,programID);
    graphicObj->drawByID(YU_GRAPHICS_SLIME,programID);
}

bool AISlime::isChoose(const glm::vec3 &start, const glm::vec3 &dir)
{
        //因為是正交矩陣，所以 inverse = transpose
    glm::mat4 tp = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1),glm::radians(angY),glm::vec3(0.0,1.0,0.0)) * glm::scale(glm::mat4(1), {sz,sz,sz});

    float min_pnt = 1e9;
    const vector<glm::vec3>& tpvec = graphicObj->getVerticesByID(YU_GRAPHICS_SLIME);
    for(int i = 0; i < tpvec.size(); i += 3){
        glm::vec3 a = tp*glm::vec4{tpvec[i],1};
        glm::vec3 b = tp*glm::vec4{tpvec[i+1],1};
        glm::vec3 c = tp*glm::vec4{tpvec[i+2],1};
        glm::vec3 U = b - a;
        glm::vec3 V = c - a;
        float det_mtx = dot(V, cross(U, - dir));
        float s = dot(start - a, cross(U, -dir)) / det_mtx;
        float t = dot(V, cross(start - a, -dir)) / det_mtx;
        float r = dot(V, cross(U, start - a)) / det_mtx;

        if(s >= 0 && t >= 0 && s + t <= 1 && r >= 0){
            return 1;
        }
    }
    return 0;
}

void AISlime::shoot(const glm::vec3 &start, const glm::vec3 &dir)
{
    if(isChoose(start,dir)){

    }
}

void AISlime::fericious_fsm()
{

}

void AISlime::timid_fsm()
{

}

void AISlime::normal_fsm()
{
    move_animation();
    //move(MOVETOROBOT);
    standBy();
    //update();
    //move_animation();
}

void AISlime::update()
{

}

void AISlime::move(int TYPE)
{
    //位移
    float tpx = fpperspective->pos[0] - pos[0];
    float tpz = fpperspective->pos[2] - pos[2];
    float m = sqrt(tpx*tpx + tpz*tpz);
    float x = tpx/m,z = tpz/m;
    if(TYPE == MOVETOROBOT){
        x *=  moveToRobotOffset;
        z *=  moveToRobotOffset;
    }if(TYPE == GOAWAY){
        x *= -goAwayOffset;
        z *= -goAwayOffset;
    }
    if(abs(tpx) < 0.1) x = 0;
    if(abs(tpz) < 0.1) z = 0;
    
    bool canx = 0,canz = 0;
    if(sceneJungle->detectCollision(pos[0]+x,pos[2],SJ_ROBOT) == 0) canx = 1;
    if(sceneJungle->detectCollision(pos[0],pos[2]+z,SJ_ROBOT) == 0) canz = 1;
    if(canx && canz) {
        pos[0] += x;
        pos[2] += z;
    }else if(canx){
        if(sceneJungle->detectCollision(pos[0]+3*x,pos[2],SJ_ROBOT) == 0) pos[0] += 3*x;
        else pos[0] += x;
    }else if(canz){
        if(sceneJungle->detectCollision(pos[0],pos[2]+3*z,SJ_ROBOT) == 0) pos[2] += 3*z;
        else pos[2] += z;
    }
    
    //旋轉
    vec3 a{fpperspective->pos[0] - pos[0],0,fpperspective->pos[2] - pos[2]};
    vec3 b{0,0,1};
    angY = glm::dot(a,b)/(sqrt(a[0]*a[0] + a[2]*a[2])*sqrt(b[0]*b[0] + b[2]*b[2]));
    angY = acos(angY) * 180 / 3.1415926;  //弧度轉角度
    if(a[0] < 0) angY = 360 - angY;
    if(TYPE == GOAWAY) angY += 180;
    angY = glm::radians(ceil(angY));  //轉回弧度
}

void AISlime::standBy()
{
    if(standByState == 0){
        standByDir = {rand()%100/100.0,0,rand()%100/100.0};
        if(rand()%2) standByDir[0]*=-1;
        if(rand()%2) standByDir[2]*=-1;
        standByState = 0;
        
        vec3 a{standByDir[0],0,standByDir[2]};
        vec3 b{0,0,1};
        angY = glm::dot(a,b)/(sqrt(a[0]*a[0] + a[2]*a[2])*sqrt(b[0]*b[0] + b[2]*b[2]));
        angY = acos(angY) * 180 / 3.1415926;  //弧度轉角度
        if(a[0] < 0) angY = 360 - angY;
        angY += 180;
        angY = glm::radians(ceil(angY));  //轉回弧度
    }
    float tpx = standByDir[0];
    float tpz = standByDir[2];
    float m = sqrt(tpx*tpx + tpz*tpz);
    float x = -tpx/m,z = -tpz/m;
    x *= standByOffset;
    z *= standByOffset;
    bool canx = 0,canz = 0;
    if(sceneJungle->detectCollision(pos[0]+x,pos[2],SJ_ROBOT) == 0) canx = 1;
    if(sceneJungle->detectCollision(pos[0],pos[2]+z,SJ_ROBOT) == 0) canz = 1;
    if(canx && canz) {
        pos[0] += x;
        pos[2] += z;
    }else if(canx){
        if(sceneJungle->detectCollision(pos[0]+3*x,pos[2],SJ_ROBOT) == 0) pos[0] += 3*x;
        else pos[0] += x;
    }else if(canz){
        if(sceneJungle->detectCollision(pos[0],pos[2]+3*z,SJ_ROBOT) == 0) pos[2] += 3*z;
        else pos[2] += z;
    }else{
        standByState = 0;
        return;
    }
    standByState++;
    standByState %= 200;
}

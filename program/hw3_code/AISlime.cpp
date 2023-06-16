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
    if(textureID == YU_SLIME_FIRE) {
        attackRange = 15;
        detectRange = 50;
        type = FIRE;
    }
    else if(textureID == YU_SLIME_WATER) {
        attackRange = 1000;
        detectRange = 80;
        type = WATER;
    }else if(textureID == YU_SLIME_LIGHT){
        type = LIGHT;
    }
}

void AISlime::FSM()
{
    if(AIID == FEROCIOUS) fericious_fsm();
    else if(AIID == TIMID) timid_fsm();
    else if(AIID == NORMAL) normal_fsm();
}

void AISlime::attack()
{
    //init y
    moveAnimationState = 0;
    pos[1] = 0;
    flag = 0;
// move_animation();
    attackAngY+=0.5;
    if(attackAngY >= 360) attackAngY = 0;
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

    if(state == ATTACK){
        if(type == FIRE){
            // tp = glm::translate(glm::mat4(1), {pos[0],0.1,pos[2]}) * glm::rotate(glm::mat4(1),angY,glm::vec3(0.0,1.0,0.0)) * glm::scale(glm::mat4(1), {40,40,40});
            // glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);
            // myTex->red->use(programID);
            // graphicObj->circle->draw(programID);


            glPushMatrix();
 
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            tp = glm::translate(glm::mat4(1), {pos[0],0.1,pos[2]}) * glm::rotate(glm::mat4(1),(float)attackAngY,glm::vec3(0.0,1.0,0.0)) * glm::scale(glm::mat4(1), {30,30,30});
            glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);
            myTex->fire->use(programID);
            graphicObj->square->draw(programID);

            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND); 

            glPopMatrix();
        }
    }
}

bool AISlime::isChoose()
{
    vec3 start = fpperspective->pos;
    vec3 dir = fpperspective->dir;

    //因為是正交矩陣，所以 inverse = transpose
    glm::mat4 tp = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1),angY,glm::vec3(0.0,1.0,0.0)) * glm::scale(glm::mat4(1), {sz,sz,sz});

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

void AISlime::shoot()
{
    //cout << "shoot\n";
    blood -= 20;
    injuried = 1; //受到攻擊
    cout << name << " ";
    cout << blood << "\n";
    // if(isChoose()){
    //     blood -= 20;
    //     injuried = 1; //受到攻擊
    //     cout << "slime!\n";
    //     cout << blood << "\n";
    // }
}

void AISlime::fericious_fsm()
{
    float dis = getDis(pos[0],pos[2],fpperspective->pos[0],fpperspective->pos[2]);
    if(state == STANDBY){
        if(dis <= attackRange){
            state = ATTACK;
            standByState = 0;
        }else if(dis <= detectRange){
            state = MOVETOROBOT;
            standByState = 0;
        }else{
            standBy();
        }
    }else if(state == MOVETOROBOT){
        if(dis <= attackRange) state = ATTACK;
        else move();
    }else if(state == ATTACK){
        if(dis >= attackRange) state = QUICK_MOVETOROBOT;
        else attack();
    }else if(state == QUICK_MOVETOROBOT){
        if(dis <= attackRange) state = ATTACK;
        else move();
    }
}

void AISlime::timid_fsm()
{
    float dis = getDis(pos[0],pos[2],fpperspective->pos[0],fpperspective->pos[2]);   
    if(state == STANDBY){
        if(injuried) state = QUICK_RUNAWAY;
        else if(dis < 30){
            state = RUNAWAY;
        }else standBy();
    }else if(state == RUNAWAY){
        if(injuried) state = QUICK_RUNAWAY;
        else if(dis > 50) {
            state = STANDBY;
            injuried = 0;
        }
        else move();
    }else if(state == QUICK_RUNAWAY){
        if(dis > 100) {
            state = STANDBY;
            injuried = 0;
        }else move();
    }
}

void AISlime::normal_fsm()
{
    float dis = getDis(pos[0],pos[2],fpperspective->pos[0],fpperspective->pos[2]);   
    if(state == STANDBY){
        if(injuried){
            state = MOVETOROBOT;
        }else standBy();
    }else if(state == MOVETOROBOT){
        if(blood < 30)state = QUICK_RUNAWAY;
        else if(blood < 50)  state = RUNAWAY;
        else if(dis < attackRange) state = ATTACK;
        else move();
    }else if(state == ATTACK){
        if(blood < 30) state = QUICK_RUNAWAY;
        else if(blood < 50)  state = RUNAWAY;
        else if(dis > attackRange) state = MOVETOROBOT;
        else attack();
    }else if(state == RUNAWAY){
        if(dis < 10) state = QUICK_RUNAWAY;
        else if(dis >= 50) {
            state = STANDBY;
            injuried = 0;
        }
        else move();
    }else if(state == QUICK_RUNAWAY){
        if(dis >= 50) {
            state = STANDBY;
            injuried = 0;
        }
        else move();
    }
}

void AISlime::update()
{

}

void AISlime::move()
{
    move_animation();
    //位移
    float tpx = fpperspective->pos[0] - pos[0];
    float tpz = fpperspective->pos[2] - pos[2];
    float m = sqrt(tpx*tpx + tpz*tpz);
    float x = tpx/m,z = tpz/m;
    if(state == MOVETOROBOT){
        x *=  moveToRobotOffset;
        z *=  moveToRobotOffset;
    }else if(state == RUNAWAY){
        x *= -runAwayOffset;
        z *= -runAwayOffset;
    }else if(state == QUICK_MOVETOROBOT){
        x *=  (moveToRobotOffset*5);
        z *=  (moveToRobotOffset*5);
    }else if(state == QUICK_RUNAWAY){
        x *=  (-runAwayOffset*5);
        z *=  (-runAwayOffset*5);
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
    if(state == RUNAWAY || state == QUICK_RUNAWAY) angY += 180;
    angY = glm::radians(ceil(angY));  //轉回弧度

}

void AISlime::standBy()
{
    move_animation();
    if(blood <= 99.95) {
        blood += 0.05;
    }
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

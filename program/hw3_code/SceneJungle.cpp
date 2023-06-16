#include "SceneJungle.h"
#include "MyRobot.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
extern Billboard* billboard;
extern MyRobot *myRobot;
extern FPPerspective* fpperspective;
extern int scene;
SceneJungle::SceneJungle()
{
    dirLight = new Light(0,400,20,400,
                        -1,-1,-1,
                        1,1,1,
                        1,1.8f);

    spotLight = new Light(1,0,0,0,
                            0,-1,0,
                            1,1,1,
                            1,0);
    vec3 tppos;
    for(int i=0;i<10;i++){
        if(rand()%2) tppos={rand()%200,0,rand()%200};
        else tppos={160 + rand()%200,0, 160 + rand()%200};
        slime.push_back(AISlime(slimeTex[rand()%3],slimeAI[rand()%3],tppos,3+rand()%3));
        slime[i].name = i+'0';
    }
    //fireslime = new AISlime(YU_SLIME_FIRE,FEROCIOUS,{160,0,160},5);
    
}


void SceneJungle::draw(float *eyeMtx, int programID)
{
    float objMtx[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    {   //地塊
        glPushMatrix();
        glTranslatef(0, -10.5, 0);
        glScalef(200, 10, 200);
        glPushMatrix();
        glTranslatef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->cube->draw(programID);
        glPopMatrix();
        glPopMatrix();
        //花
        glPushMatrix();
        for (float i = 20; i < 200; i += 40) {
            for (float j = 20; j < 200; j += 40) {
                glPushMatrix();
                glTranslatef(i, 0, j);
                glScalef(40, 1, 40);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->grass_light->use(programID);
                graphicObj->square->draw(programID);
                glPopMatrix();
            }
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(160, -10.5, 160);
        glScalef(200, 10, 200);
        glPushMatrix();
        glTranslatef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->cube->draw(programID);
        glPopMatrix();
        glPopMatrix();
        //石頭
        glPushMatrix();
        for (float i = 180; i < 360; i += 40) {
            for (float j = 180; j < 360; j += 40) {
                glPushMatrix();
                glTranslatef(i, 0, j);
                glScalef(40, 1, 40);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->grass_light->use(programID);
                graphicObj->square->draw(programID);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    {
        for(int i=0;i<slime.size();i++){
            slime[i].draw(programID);
        }
    }
    attackSlime();
    glm::mat4 tp;
    for(int i=0;i<bullet.size();i++){
        tp = glm::translate(glm::mat4(1), bullet[i].pos) * glm::scale(glm::mat4(1), {2,2,2});
        glUniformMatrix4fv(2, 1, GL_FALSE, &tp[0][0]);
        myTex->black->use(programID);
        graphicObj->solidsphere->draw(programID);
    }
    addSlime();
}

void SceneJungle::useLight()
{
    dirLight->use();
    spotLight->use(0,0,0);
}

bool SceneJungle::detectCollision(float x, float z, int MODEL)
{
    if(MODEL == SJ_ROBOT){
        if(x <= 0 || (x>=200 && z <= 160) || z <= 0 || (z >=200 && x <= 160)) return 1;
        if(x >= 360 || z >= 360 || (x>=200 && z<=160) || (x<=160 && z>=200)) return 1;
        return 0;
    }
}
void SceneJungle::attackSlime(){
    for(int i=0;i<bullet.size();i++){
        bool isShoot = 0;
        for(int j=0;j<slime.size();j++){
            if(getDis(bullet[i].pos,{slime[j].pos[0],slime[j].pos[1]+0.875*slime[j].sz,slime[j].pos[2]}) <= slime[j].sz){
                slime[j].shoot();
                if(slime[j].blood <= 0) {
                    slime.erase(slime.begin()+j);
                }
                bullet.erase(bullet.begin()+i);
                isShoot = 1;
                break;
            }
        }
        if(isShoot == 0){
            bullet[i].pos+=bullet[i].dir;
            bullet[i].state++;
        }
        if(bullet[i].state >= 100)bullet.erase(bullet.begin()+i);
    }
}
//slime.push_back(AISlime(YU_SLIME_FIRE,NORMAL,{160,0,160},5));
void SceneJungle::addSlime(){
    slimeGenState++;
    slimeGenState%=100;
    vec3 tppos;
    if(slimeGenState == 0 && slime.size()<=10){
        if(rand()%2) tppos={rand()%200,0,rand()%200};
        else tppos={160 + rand()%200,0, 160 + rand()%200};
        slime.push_back(AISlime(slimeTex[rand()%3],slimeAI[rand()%3],tppos,1+rand()%6));
    }
};
void SceneJungle::mouseClickEvent(int btn, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        if (btn == GLUT_RIGHT_BUTTON) {

        }
        else if(btn == 3){
        }else if(btn == 4){
        }else if(btn == 0){
            bullet.push_back(BulletInfo(fpperspective->pos,fpperspective->dir));
            // waterslime->shoot();
            // lightslime->shoot();
        }
    }
    else if (state == GLUT_UP) {
    }
}

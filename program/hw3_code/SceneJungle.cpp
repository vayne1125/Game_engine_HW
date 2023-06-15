#include "SceneJungle.h"
#include "MyRobot.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
extern Billboard* billboard;
extern MyRobot *myRobot;
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
    fireslime = new AISlime(YU_SLIME_FIRE,NORMAL,{160,0,160},8);
    
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
        fireslime->draw(programID);
    }
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

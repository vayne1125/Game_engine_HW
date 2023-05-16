#include"ScenePhysicalExpFiled.h"
extern myobj* myObj;
extern mytex* myTex;
void ScenePhysicalExpFiled::draw(float *eyeMtx, int programID)
{
    float objMtx[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    {   //地塊
        glPushMatrix();
        glTranslatef(pos[0], -10.5, pos[2]);
        glScalef(floorX, floorY, floorZ);
        glPushMatrix();
        glTranslatef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray ->use(programID);
        myObj->cube->draw(programID);
        glPopMatrix();
        glPopMatrix();
        //石頭
        glPushMatrix();
        for (float i = pos[0]+7.25; i < pos[0]+232; i += 14.5) {
            for (float j = pos[2]+7.25; j < pos[2]+203; j += 14.5) {
                glPushMatrix();
                glTranslatef(i, 0, j);
                glScalef(14.5, 1, 14.5);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->stone_floor->use(programID);
                myObj->square->draw(programID);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
}

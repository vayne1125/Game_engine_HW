#include "Elf.h"
extern myobj* myObj;
extern mytex* myTex;
Elf::Elf(float x, float y, float z)
{
    pos = {x,y,z};
}
void Elf::draw(unsigned programID)
{
    float objMtx[16] = { 0 };
    glPushMatrix();
    glRotatef(angleY,0,1,0);
    {   //頭
        glPushMatrix();           
        glScalef(2, 2, 2);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_blue_main->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }

    {   //外右耳
        glPushMatrix();
        glTranslatef(0.5, 0.6, -0.2);
        glRotatef(20, 0, 0, 1);
        glScalef(0.8, 0.8, 0.4);
        {   //cube offset
            glPushMatrix();
            glTranslatef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->robot_blue_sub->use(programID);
            myObj->cube->draw(programID);
            glPopMatrix();
        }
        //內耳         
        glPushMatrix();
        glTranslatef(0.2, 0.2, 0.7);
        glScalef(0.6, 0.6, 0.4);
        {
            glPushMatrix();
            glTranslatef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->robot_gray->use(programID);
            myObj->cube->draw(programID);
            glPopMatrix();
        }
        glPopMatrix();
    }

    {   //外左耳
        glPushMatrix();
        glTranslatef(-0.5, 0.6, 0.2);
        glRotatef(-20, 0, 0, 1);
        glRotatef(180, 0, 1, 0);
        glScalef(0.8, 0.8, 0.4);
        {   //cube offset
            glPushMatrix();
            glTranslatef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->robot_blue_sub->use(programID);
            myObj->cube->draw(programID);
            glPopMatrix();
        }
        //內耳         
        glPushMatrix();
        glTranslatef(0.2, 0.2, -0.1);
        glScalef(0.6, 0.6, 0.4);
        {
            glPushMatrix();
            glTranslatef(0.5, 0.5, 0.5);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->robot_gray->use(programID);
            myObj->cube->draw(programID);
            glPopMatrix();
        }
        glPopMatrix();
    }
    {  //眼睛
        glPushMatrix();
        glTranslatef(0.4, 0.15, 1);
        glScalef(0.2, 0.4, 0.2);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->black->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.4, 0.15, 1);
        glScalef(0.2, 0.4, 0.2);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->black->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();

    }

    //腮紅
    {
        glPushMatrix();
        glTranslatef(0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->elf_red->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
                
        glPushMatrix();
        glTranslatef(-0.7, -0.2, 0.8);
        glScalef(0.4, 0.2, 0.2);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->elf_red->use(programID);
        myObj->solidsphere->draw(programID);
        glPopMatrix();
    }
    glPopMatrix();
        
}

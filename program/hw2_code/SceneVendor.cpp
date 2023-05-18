#include "SceneVendor.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
extern Billboard* billboard;

SceneVendor::SceneVendor()
{
    spotLightElf = new Elf(0,40,203);
}

void SceneVendor::draw(float* eyeMtx,int programID)
{

    float objMtx[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    {   //地塊
        glPushMatrix();
        glTranslatef(0, -10.5, 0);
        glScalef(floorX, floorY, floorZ);
        glPushMatrix();
        glTranslatef(0.5, 0.5, 0.5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->robot_gray ->use(programID);
        graphicObj->cube->draw(programID);
        glPopMatrix();
        glPopMatrix();
        //石頭
        glPushMatrix();
        for (float i = 7.25; i < 232; i += 14.5) {
            for (float j = 7.25; j < 203; j += 14.5) {
                glPushMatrix();
                glTranslatef(i, 0, j);
                glScalef(14.5, 1, 14.5);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->stone_floor->use(programID);
                graphicObj->square->draw(programID);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    
    {   //伊布
    cout << eyeMtx << "\n";
        billboard->draw(65, 70, 28.125, 15, myTex->eevee[eevee_ani],eyeMtx,programID);
    }

    {   //噴水池
        glPushMatrix();
        glTranslatef(fountainPos[0], fountainPos[1], fountainPos[2]);
        glScalef(0.3, 0.3, 0.3);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->fountain_base->use(programID);
        graphicObj->fountain_base->draw(programID);
        myTex->fountain_water->use(programID);
        graphicObj->fountain_water->draw(programID);
        glPopMatrix();
    }
    //glPushMatrix();
    //glTranslatef(100, 30, 100);
    ////glRotatef(90, 1, 0, 0);
    //glScalef(10, 10, 10);
    //glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
    //glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
    //myTex->blacksmith->use(programID);
    //graphicObj->blacksmith->draw(programID);
    //glPopMatrix();
    {   //vendor
        glPushMatrix();
        glRotatef(90, 0, 1, 0);

        glTranslatef(-24, 0, 0);
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(0, 0, 67.5 + 48.5 * (i));
            glScalef(3, 3, 3);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->wood3->use(programID);
            graphicObj->vendor_base->draw(programID);
            myTex->wood4->use(programID);
            graphicObj->vendor_col->draw(programID);
            myTex->tent->use(programID);
            graphicObj->vendor_roof->draw(programID);
            myTex->tablecloth->use(programID);
            graphicObj->vendor_desk->draw(programID);
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(-24, 0, -70);
        glPushMatrix();
        glScalef(3, 3, 3);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->wood3->use(programID);
        graphicObj->vendor_base->draw(programID);
        myTex->wood4->use(programID);
        graphicObj->vendor_col->draw(programID);
        myTex->tent->use(programID);
        graphicObj->vendor_roof->draw(programID);
        myTex->green_fabric->use(programID);
        graphicObj->vendor_desk->draw(programID);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(209.5, 0, 70);
        glPushMatrix();
        glScalef(3, 3, 3);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->wood3->use(programID);
        graphicObj->vendor_base->draw(programID);
        myTex->wood4->use(programID);
        graphicObj->vendor_col->draw(programID);
        myTex->tent->use(programID);
        graphicObj->vendor_roof->draw(programID);
        myTex->blue_fabric->use(programID);
        graphicObj->vendor_desk->draw(programID);
        glPopMatrix();
        glPopMatrix();
        {
            //籃子
            for (int i = 0; i < 5; i++) {
                glPushMatrix();
                glTranslatef(100 + 8 * i, 16, 43);

                glPushMatrix();
                glRotatef(5, 1, 0, 0);
                glScalef(23, 23, 23);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->wood2->use(programID);
                graphicObj->vendor_crate->draw(programID);
                glPopMatrix();

                for (int j = -1; j <= 1; j++) {
                    for (int k = 0; k < 2; k++) {
                        glPushMatrix();
                        glTranslatef(-1.8 + k * 3.5, 1, j * 3.5);
                        glScalef(23, 23, 23);
                        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                        if (i == 0)
                            myTex->red_apple->use(programID);
                        else if (i == 1)
                            myTex->pear->use(programID);
                        else if (i == 2)
                            myTex->orange_dark->use(programID);
                        else if (i == 3)
                            myTex->green_apple->use(programID);
                        else
                            myTex->lemon->use(programID);
                        graphicObj->apple->draw(programID);
                        glPopMatrix();
                    }
                }
                glPopMatrix();
            }
            //麵包攤
            {
                //起司盤子
                glPushMatrix();
                glTranslatef(155, 16, 44);
                glPushMatrix();
                glScalef(6, 6, 6);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->wood5->use(programID);
                graphicObj->plate->draw(programID);
                glPopMatrix();
                glPushMatrix();         //起司
                glTranslatef(-1, 0.2, 1);
                glScalef(23, 23, 23);
                glRotatef(100, 0, 1, 0);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->cheese->use(programID);
                graphicObj->cheese->draw(programID);
                glPopMatrix();
                glPopMatrix();


                //麵包盤子
                glPushMatrix();
                glTranslatef(174, 16, 44);
                glPushMatrix();
                glScalef(6.5, 4, 4);
                glRotatef(90, 0, 1, 0);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->wood5->use(programID);
                graphicObj->plate->draw(programID);
                glPopMatrix();
                //麵包
                glPushMatrix();
                glTranslatef(-2, 0.2, 2.5);
                glScalef(2.7, 2.7, 2.7);
                glRotatef(100, 0, 1, 0);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->bread->use(programID);
                graphicObj->bread2->draw(programID);
                glPopMatrix();
                glPopMatrix();
            }
            // 武器攤
            {
                // 劍
                glPushMatrix();
                glTranslatef(191, 16, 58);
                glPushMatrix();
                glScalef(4.5, 4, 4);
                glRotatef(90, 0, 1, 0);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->silver->use(programID,0.508273f, 0.508273f, 0.508273f,51.25f);
                graphicObj->sword->draw(programID);
                glPopMatrix();
                glPopMatrix();

                //弓箭
                glPushMatrix();
                glTranslatef(189, 16, 78);
                glPushMatrix();
                glScalef(7.8, 7.8, 7.8);
                //glRotatef(90, 1, 0, 0);
                //glRotatef(90, 0, 0, 1);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->wood->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
                graphicObj->bow->draw(programID);
                glPopMatrix();
                glPopMatrix();
                //弓箭
                glPushMatrix();
                glTranslatef(194, 16, 78);
                glPushMatrix();
                glScalef(7.8, 7.8, 7.8);
                //glRotatef(90, 1, 0, 0);
                //glRotatef(90, 0, 0, 1);
                glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                myTex->wood->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
                graphicObj->bow->draw(programID);
                glPopMatrix();
                glPopMatrix();
            }
            //燈籠攤
            {
                for (int i = 0; i < 3; i++) {
                    glPushMatrix();
                    glTranslatef(45, 32, 60+10*i);
                    glPushMatrix();
                    glScalef(3, 3, 3);
                    //glRotatef(90, 1, 0, 0);
                    //glRotatef(90, 0, 0, 1);
                    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                    if(i&1)
                        myTex->red_lantern2->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
                    else
                        myTex->red_lantern->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
                    graphicObj->lantern3->draw(programID);
                    glPopMatrix();
                    glPopMatrix();
                }
                for (int i = 0; i < 5; i++) {
                    glPushMatrix();
                    glTranslatef(45+(i*37 % 5 )/5 - 2.5f, 18, 55 + 8 * i);
                    glPushMatrix();
                    glScalef(108, 128, 108);
                    //glRotatef(90, 1, 0, 0);
                    //glRotatef(90, 0, 0, 1);
                    glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
                    glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
                    myTex->red_lantern->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
                    graphicObj->lantern1->draw(programID);
                    glPopMatrix();
                    glPopMatrix();
                }
            }
        }
    }
    {   //tree
        glPushMatrix();
        glTranslatef(15,0,15);
        glScalef(7, 7, 7);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->tree_round_up->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_round_btn->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(28, 0, 28);
        glScalef(4, 4, 4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_light->use(programID);
        graphicObj->tree_round_up->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_round_btn->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(205, 0, 15);
        glScalef(7, 7, 7);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->tree_conical_up ->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_conical_btn->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(220, 0, 28);
        glScalef(4, 4, 4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_light->use(programID);
        graphicObj->tree_round_up->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_round_btn->draw(programID);
        glPopMatrix();
    }
    {   //雲
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(cloud[i][0], cloud[i][1], cloud[i][2]);
            glScalef(4, 4, 4);
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->white->use(programID);
            graphicObj->cloud1->draw(programID);
            glPopMatrix();
        }
    }
    {   //spotlight
        glPushMatrix();
        glTranslatef(spotLightElf->pos[0], spotLightElf->pos[1], spotLightElf->pos[2]);
        glScalef(4, 4, 4);
        spotLightElf->draw(programID);
        glPopMatrix();
    } 


}

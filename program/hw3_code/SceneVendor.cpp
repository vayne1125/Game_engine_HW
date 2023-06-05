#include "SceneVendor.h"
#include "MyRobot.h"
extern GraphicObj* graphicObj;
extern mytex* myTex;
extern Billboard* billboard;
extern MyRobot *myRobot;
SceneVendor::SceneVendor(int programID)
{
    spotLightElf = new Elf(0,40,203);
    spotLight = new Light(1,spotLightElf->pos[0],spotLightElf->pos[1],spotLightElf->pos[2],
                            0,-1,0,
                            1,1,1,
                            1,40);
    dirLight = new Light(0,-100,50,0,
                        -1,-1,-1,
                        1,1,1,
                        1,2.3f);

    chiefOfVillage = new Robot(programID,116,0,157); 
    chiefOfVillage->setColor(myTex->orange_light_robot,myTex->orange_dark_robot);
    
    fruitMerchant = new Robot(programID,116,0,23);
    fruitMerchant->setColor(myTex->white,myTex->yellow_light_robot);
    fruitMerchant->scaleX = 3;
    fruitMerchant->scaleY = 3;
    fruitMerchant->scaleZ = 3;

    weaponDealer = new Robot(programID,205.5,0,70);
    weaponDealer->angle_y = 270;
    weaponDealer->setColor(myTex->white,myTex->yellow_light_robot);
    weaponDealer->scaleX = 3;
    weaponDealer->scaleY = 3;
    weaponDealer->scaleZ = 3;
}
void SceneVendor::useLight(){
    dirLight->use();
    spotLight->use(spotLightElf->pos[0],spotLightElf->pos[1],spotLightElf->pos[2]);
}
void SceneVendor::elfMove(){
    float tpPos[3];
    for (int i = 0; i < 3; i++) tpPos[i] = spotLightElf->pos[i];
    if (spotLightElf->dir == 2) { //前
        tpPos[2]--;
    }
    else if (spotLightElf->dir == 3) { //左
        tpPos[0]--;
    }else if (spotLightElf->dir == 0) { //後
        tpPos[2]++;
    }
    else if (spotLightElf->dir == 1) { //右
        tpPos[0]++;
    }
    if (detectCollision(tpPos[0], tpPos[1], tpPos[2],1)) {
        //cout << tpPos[0] << " " << tpPos[2] << "\n";
        int tp = 1 + rand() % 4;
        //tp += 2;
        spotLightElf->dir = (spotLightElf->dir + tp) % 4;
        spotLightElf->angleY = 90 * (spotLightElf->dir);
    }
    else {
        for (int i = 0; i < 3; i++) spotLightElf->pos[i] = tpPos[i];
    }
}
float SceneVendor::getDis(float x1, float y1, float x2, float y2) {           //算距離
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
bool SceneVendor::detectCollision(float x,float y,float z,int ITEM){
    if (z <= 48 + 6) return 1;   //第一行攤販
    if ((z <= 95 && (x <= 48 + 6 || x >= 187)) || (z <= 95 && (x <= 48 - 6 || x >= 187 - 7))) return 1;     //第二條攤販
    if (x < 0 || x > 232 || z < 0 || z > 280) return 1; //邊界
    if (z >= 203 && !(x >= 101 && x <= 131)) return 1;
    if (ITEM == 0) {
        if (getDis(x, z, fountainPos[0], fountainPos[2]) <= 29) return 1;   //噴水池
    }
    else if (ITEM == SPOTLIGHT_ELF) {
        if (getDis(x, z, fountainPos[0], fountainPos[2]) <= 5) return 1;   //噴水池
        for (int i = 0; i < cloud.size(); i++) {
            if (getDis(x, z, cloud[i][0], cloud[i][2]) <= 5) return 1;
        }
    }
    return 0;
}
void SceneVendor::animation(int ITEM){
    switch(ITEM){
        case ANIMATION_SPOTLIGHT_ELF:
            elfMove();
            break;
        case ANIMATION_EEVEE:
            eevee_ani++;
            if (eevee_ani >= 56) eevee_ani = 0;
            break;
    }    
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

    {   //地塊(往tree)
        glPushMatrix();
        glTranslatef(101.5, -10.5, 203);
        glScalef(29, floorY, 87);
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
        for (float i = 7.25; i < 29; i += 14.5) {
            for (float j = 7.25; j < 87; j += 14.5) {
                glPushMatrix();
                glTranslatef(101.5 + i, 0, 203 + j);
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
        //cout << eyeMtx << "\n";
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
        {// 水果攤
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
        }

        {//麵包攤
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
        
        {// 武器攤
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
            glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
            glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
            myTex->wood->use(programID, 0.508273f, 0.508273f, 0.508273f, 51.25f);
            graphicObj->bow->draw(programID);
            glPopMatrix();
            glPopMatrix();
        }
        
        {//燈籠攤
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
    
    {   //spotlight
        glPushMatrix();
        glTranslatef(spotLightElf->pos[0], spotLightElf->pos[1], spotLightElf->pos[2]);
        glScalef(4, 4, 4);
        spotLightElf->draw(programID);
        glPopMatrix();
    }

    {   //vendor tree
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

    {   //NPC
        //村長
        chiefOfVillage->draw(programID);
        //水果店老闆
        fruitMerchant->draw(programID);
        //武器行老闆
        weaponDealer->draw(programID);
    }
    
    {   //slime
        glPushMatrix();
        glTranslatef(fountainPos[0], fountainPos[1], fountainPos[2]+40);
        glPushMatrix();
        glScalef(5, 5, 5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->slime_water->use(programID);
        graphicObj->slime->draw(programID);
glPopMatrix();
        glTranslatef(0,0,20);
        glPushMatrix();
        glScalef(5, 5, 5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->slime_fire->use(programID);
        graphicObj->slime->draw(programID);
glPopMatrix();
        glTranslatef(0,0,20);
        glPushMatrix();
        glScalef(5, 5, 5);
        
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->slime_light->use(programID);
        graphicObj->slime->draw(programID);
        glPopMatrix();
        glPopMatrix();

    }

    {   //前往叢林 tree
        glPushMatrix();
        glTranslatef(105, 0, 275);
        glScalef(4, 4, 4);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_light->use(programID);
        graphicObj->tree_round_up->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_round_btn->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(116,0,285);
        glScalef(5, 5, 5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->tree_round_up->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_round_btn->draw(programID);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(128, 0, 275);
        glScalef(5, 5, 5);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->grass_dark->use(programID);
        graphicObj->tree_conical_up ->draw(programID);
        myTex->coffee_dark->use(programID);
        graphicObj->tree_conical_btn->draw(programID);
        glPopMatrix();
    }
    {   //轉移魔法陣
        glPushMatrix();
 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTranslatef(116, 1, 270);  
        glScalef(20, 1, 20);
        glGetFloatv(GL_MODELVIEW_MATRIX, objMtx);
        glUniformMatrix4fv(2, 1, GL_FALSE, objMtx);
        myTex->magic_circle->use(programID);
        graphicObj->square->draw(programID);

        glDisable(GL_ALPHA_TEST);
        glDisable(GL_BLEND); 

        glPopMatrix();
    }
}
void SceneVendor::keyEvent(unsigned char key){
    if (key == '0') {
        isDirLightOpen ^= 1;
        if(isDirLightOpen) dirLight->setStr(2.3);
        else dirLight->setStr(0);
    }else if((int)key == 13){  //轉移魔法陣 
        if(getDis(myRobot->pos[0],myRobot->pos[2],116,270) <= 10){
            //todo
        }
    }
}
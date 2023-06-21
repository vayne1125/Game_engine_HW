#include "UI.h"
#include "MyRobot.h"
#include "std_image.h"
#include "AISlime.h"
#include "SceneJungle.h"
extern mytex* myTex;
extern GraphicObj* graphicObj;
extern MyRobot *myRobot;
extern SceneJungle* sceneJungle;
extern unsigned int programID;
UI::UI(float asp){
    aspect = asp;
    unsigned char* image_data;
    int image_width, image_height, nrChannels;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(4, head);
    image_data = stbi_load("../texture/head.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, head[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("../texture/waterslime_head.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, head[WATER]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("../texture/fireslime_head.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, head[FIRE]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    image_data = stbi_load("../texture/lightslime_head.png", &image_width, &image_height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, head[LIGHT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }
}
void UI::draw()
{
    //準心
    float f = 0.1 / aspect;
    glColor3f(1, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-f / 2.0, 0);
    glVertex2f(f / 2.0, 0);
    glVertex2f(0, -0.05);
    glVertex2f(0, 0.05);
    glEnd();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(-0.98, -0.95);
    glVertex2f(-0.98, -0.6);
    glVertex2f(-0.52, -0.6);
    glVertex2f(-0.52, -0.95);
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, head[3]);

    glColor4f(1,1,1,0.8);    
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); 
    glVertex2f(-0.96, -0.91);
    glTexCoord2f(1, 0); 
    glVertex2f(-0.96, -0.64);
    glTexCoord2f(0, 0); 
    glVertex2f(-0.84, -0.64);
    glTexCoord2f(0, 1); 
    glVertex2f(-0.84, -0.91);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
//----------------------------------------------------
    glColor3f(0,0,0);
    std::string s = "Lv.";
    int tpm = myRobot->level;
    string tps = "";
    while(tpm > 0){
        tps += (tpm%10)+'0';
        tpm/=10;
    }
    reverse(tps.begin(),tps.end());
    s += tps;
    s += " A l l u k a";
    glRasterPos2f(-0.83, -0.69);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }
//----------------------------------------------------
    s = "blood";
    glRasterPos2f(-0.83, -0.76);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }    

    glEnable(GL_BLEND);
    glColor4f(1,1,1,0.8);
    glBegin(GL_QUADS);
    glVertex2f(-0.765, -0.725);
    glVertex2f(-0.565, -0.725);
    glVertex2f(-0.565, -0.765);
    glVertex2f(-0.765, -0.765);
    glEnd();
    
    float tp = myRobot->getBlood()*0.2/200.0;
    glColor4f(1,0,0,0.5);
    glBegin(GL_QUADS);
    glVertex2f(-0.765, -0.725);
    glVertex2f(-0.765 + tp, -0.725);
    glVertex2f(-0.765 + tp, -0.765);
    glVertex2f(-0.765, -0.765);
    glEnd();

    glDisable(GL_BLEND);
//----------------------------------------------------
    glColor3f(0,0,0);
    s = "money: ";
    tpm = myRobot->money;
    tps = "";
    while(tpm > 0){
        tps += (tpm%10)+'0';
        tpm/=10;
    }
    reverse(tps.begin(),tps.end());
    s += tps;
    glRasterPos2f(-0.83, -0.82);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }   
//----------------------------------------------------
    s = "exp";
    glRasterPos2f(-0.83, -0.885);
    for (int j = 0; j < s.size(); j++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
    }


    glEnable(GL_BLEND);
    glColor4f(1,1,1,0.8);
    glBegin(GL_QUADS);
    glVertex2f(-0.78, -0.86);
    glVertex2f(-0.565, -0.86);  //0.565 
    glVertex2f(-0.565, -0.89);
    glVertex2f(-0.78, -0.89);
    glEnd();
    
    tp = myRobot->exp*0.215/(myRobot->level*10);
    glColor4f(1,1,0,0.5);
    glBegin(GL_QUADS);
    glVertex2f(-0.78, -0.86);
    glVertex2f(-0.78 + tp, -0.86);
    glVertex2f(-0.78 + tp, -0.89);
    glVertex2f(-0.78, -0.89);
    glEnd();
//-----------------------------------slime-----------
    if(sceneJungle->haveChooseSlime){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
        glBegin(GL_QUADS);
        glVertex2f(-0.98, 0.95);
        glVertex2f(-0.98, 0.75);
        glVertex2f(-0.58, 0.75);
        glVertex2f(-0.58, 0.95);
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, head[sceneJungle->chooseSlime->type]);
        
        glColor4f(1,1,1, 0.8);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); 
        glVertex2f(-0.96, 0.93);
            glTexCoord2f(0, 1); 
        glVertex2f(-0.96, 0.77);
            glTexCoord2f(1, 1); 
        glVertex2f(-0.82, 0.77);
            glTexCoord2f(1, 0); 
        glVertex2f(-0.82, 0.93);
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

    //--------------------------------------name--------------
        glColor3f(0,0,0);
        std::string s = "Lv.1 ";
        s += sceneJungle->chooseSlime->name;
        glRasterPos2f(-0.81, 0.898);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
    //-----------------------------------------ai-----------
        s = "AI: " + sceneJungle->chooseSlime->AI_name;
        glRasterPos2f(-0.81, 0.838);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }


    //----------------------------------------blood---------
        s = "blood";
        glRasterPos2f(-0.81, 0.778);
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }    

        glEnable(GL_BLEND);
        glColor4f(1,1,1,0.8);
        glBegin(GL_QUADS);
        glVertex2f(-0.81, 0.77);
        glVertex2f(-0.61, 0.77);
        glVertex2f(-0.61, 0.76);
        glVertex2f(-0.81, 0.76);
        glEnd();
        
        float tp = sceneJungle->chooseSlime->getBlood()*0.2/100.0;
        glColor4f(1,0,0,0.5);
        glBegin(GL_QUADS);
        glVertex2f(-0.81, 0.77);
        glVertex2f(-0.81 + tp, 0.77);
        glVertex2f(-0.81 + tp, 0.76);
        glVertex2f(-0.81, 0.76);
        glEnd();

        glDisable(GL_BLEND);

    }
//-----------------------------------訊息版-----------
if(showMsg){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(208 / 255.0, 159 / 255.0, 159 / 255.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(-0.98, -0.05);
    glVertex2f(-0.98, -0.55);
    glVertex2f(-0.4, -0.55);
    glVertex2f(-0.4, -0.05);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0,0,0);
    int p = 0;
    for (auto i = msg.begin();i != msg.end();i++)
    {
        s = *i;
        //cout << *i << "\n";
        glRasterPos2f(-0.968, -0.12 - p * 0.08); //-0.07 - 0.05 
        for (int j = 0; j < s.size(); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, int(s[j]));
        }
        p++;
    }
    showMsgState++;
    showMsgState %= 500;
    if(showMsgState == 499) {
        showMsg = 0;
        showMsgState = 0;
    }
}
}
void UI::addMsg(int type,int slimename){
    string str = "",tps = "";
    int tpm = 0;
    if(type == ATTACK_SLIME){
        tpm = myRobot->bulletAtk;
        tps = "";
        while(tpm > 0){
            tps += (tpm%10)+'0';
            tpm/=10;
        }
        reverse(tps.begin(),tps.end());
        str = string("Deal ") + tps + string(" points of damage to the ");
    }else if(type == ATTACK_ROBOT){
        str = "Taking damage from the ";
    }

    if(slimename == FIRE){
        str += "Fire Slime";
    }else if(slimename == LIGHT){
        str += "Light Slime";
    }else if(slimename == WATER){
        str += "Water Slime";
    }

    if(type == ATTACK_SLIME) str += "!";
    else if(type == ATTACK_ROBOT) str += "'s assault.";
    else if(type == SLIME_DIE) str += " is die.";
    msg.push_back(str);
    if(msg.size() >= 7 ) msg.pop_front();
    showMsg = 1;
}
void UI::addMsg(int type){
    string str;
    if(type==LEVEL_UP) str = "Level up!!";
    msg.push_back(str);
    if(msg.size() >= 7 ) msg.pop_front();
    showMsg = 1;
}
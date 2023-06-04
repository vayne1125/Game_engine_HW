#include"ScenePhysicalExpFiled.h"
extern GraphicObj* graphicObj;
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
        graphicObj->cube->draw(programID);
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
                graphicObj->square->draw(programID);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
    
    int pt = clock();
    float dt = std::min((pt - pretime) / 1000.f, 0.01f);
    pretime = pt;

    for(int i=0;i<3;i++){
        object[i] -> update(dt);
        object[i] -> draw(programID);
    }
    
}
void ScenePhysicalExpFiled::keyEvent(unsigned char key){
    if (key == 'z' || key == 'Z')
    {
        chooseObject->switchDragforce();
    }
    if(key == 'x' || key == 'X'){
        chooseObject->switchGravity();
    }
    if(key == 't'){
        chooseObject->stopMove();
    }else if(key == 'T'){
        for(auto i:object) i->stopMove();
    }else if(key == 'r'){
        for(int i = 0;i<3; i++) {
            if(object[i]->name == chooseObject->name) chooseObject->reset();
        }
    }else if(key == 'R'){
        for(int i = 0;i<3; i++) object[i]->reset();
    }
    if(key == '+' || key == '='){
        force++;
    }else if(key == '-'){
        force = fmax(1,--force);
    }

}
ScenePhysicalExpFiled::ScenePhysicalExpFiled(){
    object[0] = new Object(YU_GRAPHICS_CUBE,YU_PHYSICS_CUBE,YU_SLIME_CUBE,{5, 5, 5}, 1, 0.2);
    object[0] -> setPos(70,20,250);
    object[0] -> setName("cube");
    
    object[1] = new Object(YU_GRAPHICS_SPHERE,YU_PHYSICS_SPHERE,YU_SLIME_SPHERE_PHY,10,1,0.01f);
    object[1] -> setPos(156,30,250);
    object[1] -> setName("sphere");
    object[1]->switchGravity();

    object[2] = new Object(YU_GRAPHICS_CLOUD,YU_PHYSICS_IRREGULAR,YU_SLIME_CUBE,{4,4,4},1);
    object[2] -> setPos(116,40,250);
    object[2] -> setName("cloud");
    
    chooseObject = new Object();  
    pretime = clock();  
}
#include "Light.h"
Light::Light(int type_, float pos_x_, float pos_y_, float pos_z_, float dir_x_, float dir_y_, float dir_z_, float color_r_, float color_g_, float color_b_, int expo_, float str_)
{
    type = type_;
    pos_x = pos_x_;
    pos_y = pos_y_;
    pos_z = pos_z_;
    dir_x = dir_x_;
    dir_y = dir_y_;
    dir_z = dir_z_;
    color_r = color_r_;
    color_g = color_g_;
    color_b = color_b_;
    str = str_;
    expo = expo_;
}
void Light::setStr(float s)
{
    str = s;
}
void Light::use()
{
    if(type == 0){
        glUniform4f(DIR_LIGHT_POS, pos_x, pos_y, pos_z, 0);
        glUniform3f(DIR_LIGHT_DIR, dir_x, dir_y, dir_z);
        glUniform3f(DIR_LIGHT_COLOR, color_r, color_g, color_b);
        glUniform1f(DIR_LIGHT_EXPO, expo);
        glUniform1f(DIR_LIGHT_STR, str); //1.7
    }else{
        glUniform4f(SPOT_LIGHT_STR, pos_x, pos_y, pos_z, 1);
        glUniform3f(SPOT_LIGHT_STR, dir_x, dir_y, dir_z);
        glUniform3f(SPOT_LIGHT_STR, color_r, color_g, color_b);
        glUniform1f(SPOT_LIGHT_STR, expo);  // 0 point
        glUniform1f(SPOT_LIGHT_STR, str); 
    }
}

void Light::use(float x, float y, float z)
{
    if(type==0){
        glUniform4f(DIR_LIGHT_POS, x, y, z, 0);
        glUniform3f(DIR_LIGHT_DIR, dir_x, dir_y, dir_z);
        glUniform3f(DIR_LIGHT_COLOR, color_r, color_g, color_b);
        glUniform1f(DIR_LIGHT_EXPO, expo);
        glUniform1f(DIR_LIGHT_STR, str); //1.7
    }
    else{
        glUniform4f(SPOT_LIGHT_POS, x, y, z, 1);  // 0 平行
        glUniform3f(SPOT_LIGHT_DIR, dir_x, dir_y, dir_z);
        glUniform3f(SPOT_LIGHT_COLOR, color_r, color_g, color_b);
        glUniform1f(SPOT_LIGHT_EXPO, expo);                                  // 0 point
        glUniform1f(SPOT_LIGHT_STR, str);       
    }
}

// glUniform4f(DIR_LIGHT_POS, -100, 50, 0, 0);
// glUniform3f(DIR_LIGHT_DIR, -1, -1, -1);
// glUniform3f(DIR_LIGHT_COLOR, 1, 1, 1);
// glUniform1f(DIR_LIGHT_EXPO, 1);
// glUniform1f(DIR_LIGHT_STR, dirLightStr); //1.7

// glUniform4f(SPOT_LIGHT_POS, sceneVendor->spotLightElf->pos[0], sceneVendor->spotLightElf->pos[1], sceneVendor->spotLightElf->pos[2], 1);  // 0 平行
// glUniform3f(SPOT_LIGHT_DIR, 0, -1, 0);
// glUniform3f(SPOT_LIGHT_COLOR, 1, 1, 1);
// glUniform1f(SPOT_LIGHT_EXPO, 1);                                  // 0 point
// glUniform1f(SPOT_LIGHT_STR, 40);

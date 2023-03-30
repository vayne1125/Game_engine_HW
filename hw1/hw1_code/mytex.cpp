#include "mytex.h"
#include "std_image.h"
void texture::use(unsigned int programID,float sper, float speg, float speb,float shine)
{
    glUniform1f(9, shine);                     //shine
    glUniform3f(12, sper, speg, speb);         //specular
    glBindTexture(GL_TEXTURE_2D, textureID);
}
void texture::use(unsigned int programID)
{
    glUniform1f(9, 0);                     //shine
    glUniform3f(12, 0, 0, 0);              //specular
    glBindTexture(GL_TEXTURE_2D, textureID);
}
texture::texture(const string& fname, unsigned int programID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char color[4] = { 255,255,255,255 };

	int image_width, image_height, nrChannels;
    unsigned char* image_data;
    image_data = stbi_load(fname.c_str(), &image_width, &image_height, &nrChannels, 0);
    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    glUniform1i(3, 0);  //tex
}

texture::texture(int r, int g, int b, int a, unsigned int programID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char color[4] = { r,g,b,a };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glUniform1i(glGetUniformLocation(programID, "tex"), 0);
}

mytex::mytex(unsigned int programID)
{
    //red = new texture(".\\texture\\a.png", programID);
    red = new texture(255,0,0,255, programID);
    //red = new texture(255,0,0,255, programID);
    black = new texture(0, 0, 0, 255, programID);
    white = new texture(255, 255, 255, 255, programID);
    robot_blue_main = new texture(219, 255, 255, 255, programID);
    robot_blue_sub = new texture(173, 214, 255, 255, programID);
    robot_blue_eye = new texture(128, 128, 255, 255, programID);
    robot_pink_eye = new texture(255, 122, 189, 255, programID);
    robot_gray = new texture(167, 167, 167, 255, programID);
    robot_gray_dark = new texture(127, 127, 127, 255, programID);
    elf_red = new texture(255,168,212,255, programID);
    for (int i = 0; i <= 55; i++) {
        string fname = ".\\texture\\eevee_ani_", s2 = "";
        int tp = i;
        while (tp) {
            s2 += (tp % 10) + '0';
            tp /= 10;
        }
        reverse(s2.begin(), s2.end());
        if (i == 0) s2 = "0";
        string s3 = ".png";
        fname = fname + s2 + s3;
        eevee[i] = new texture(fname.c_str(), programID);
    }
    magic_wand_wood = new texture(158, 79, 0, 255, programID);
    fountain_base = new texture(".\\texture\\fountain_base.jpg", programID);
    fountain_water = new texture(".\\texture\\fountain_water.jpg", programID);
    grass_dark = new texture(1, 152, 89,255, programID);
    grass_light = new texture(204, 255, 204,255, programID);
    coffee_dark = new texture(141,84,28,255, programID);
    coffee_light = new texture(162, 92, 21, 255, programID);
    flower = new texture(".\\texture\\flower.png", programID);
    yellow_light = new texture(255, 255, 204, 255, programID);
    yellow_dark = new texture(173, 142, 0, 255, programID);
    red_dark = new texture(117, 0, 0, 255, programID);
    stone_floor = new texture(".\\texture\\stone_floor.jpg", programID);
    wood = new texture(".\\texture\\wood.jpg", programID);
    wood2 = new texture(".\\texture\\wood2.jpg", programID);
    wood3 = new texture(".\\texture\\wood3.jpg", programID);
    wood4 = new texture(".\\texture\\wood4.jpg", programID);
    wood5 = new texture(".\\texture\\wood5.jpg", programID);
    tablecloth = new texture(".\\texture\\tablecloth.jpg", programID);
    tent = new texture(".\\texture\\tent.jpg", programID);
    orange_dark = new texture(255, 165, 0, 255, programID);
    bread = new texture(".\\texture\\bread.jpg", programID);
    cheese = new texture(".\\texture\\cheese.jpg", programID);
    red_apple = new texture(".\\texture\\red_apple.jpg", programID);
    green_apple = new texture(".\\texture\\green_apple.jpg", programID);
    pear = new texture(".\\texture\\pear.jpg", programID);
    lemon = new texture(".\\texture\\lemon.jpg", programID);
    silver = new texture(".\\texture\\sliver.jpg", programID);
    blue_fabric = new texture(".\\texture\\blue_fabric.jpg", programID);
    green_fabric = new texture(".\\texture\\green_fabric.jpg", programID);
    red_lantern = new texture(".\\texture\\red_lantern.jpg", programID);
    red_lantern2 = new texture(".\\texture\\red_lantern2.jpg", programID);

    //orange_dark, * orange_light, * dark_red, * orgred_dark, * orgred_light;
    //blacksmith = new texture(".\\texture\\black_smith.png", programID);
    //a = new texture(".\\texture\\pool.jpg", programID);
}
    

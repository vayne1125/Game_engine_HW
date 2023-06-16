#include "GraphicObj.h"
struct vertex {
    float a, b, c;
    vertex(float a_ = 0, float b_ = 0, float c_ = 0) {
        a = a_;
        b = b_;
        c = c_;
    }
};
GraphicObj::GraphicObj(unsigned int programID)
{
    
    solidsphere = getMesh("../model/solidsphere.obj", programID,0);
    cube = getMesh("../model/cube.obj", programID,0);
    cylinder = getMesh("../model/cylinder.obj", programID,0);
    solidtorus_5_025 = getMesh("../model/solidtorus_0.5_0.025.obj", programID,0);
    fountain_base = getMesh("../model/fountain_base.obj", programID,0);
    fountain_water = getMesh("../model/fountain_water.obj", programID,0);
    blacksmith = getMesh("../model/Blacksmith.obj", programID,0);
    square = getMesh("../model/square.obj", programID,0);
    vendor_base = getMesh("../model/vendor_base.obj", programID,0);
    vendor_col = getMesh("../model/vendor_col.obj", programID,0);
    vendor_roof = getMesh("../model/vendor_roof.obj", programID,0);
    vendor_desk = getMesh("../model/vendor_desk.obj", programID,0);
    tree_round_up = getMesh("../model/tree_round_up.obj", programID,0);
    tree_round_btn = getMesh("../model/tree_round_btn.obj", programID,0);
    tree_conical_up = getMesh("../model/tree_conical_up.obj", programID,0);
    tree_conical_btn = getMesh("../model/tree_conical_btn.obj", programID,0);
    cloud1 = getMesh("../model/cloud1.obj", programID,0);
    vendor_crate = getMesh("../model/vendor_crate.obj", programID,0);
    apple = getMesh("../model/apple.obj", programID,0);
    cheese = getMesh("../model/cheese.obj", programID,0);
    bread2 = getMesh("../model/bread2.obj", programID,0);
    plate = getMesh("../model/plate.obj", programID,0);
    sword = getMesh("../model/sword.obj", programID,0);
    bow = getMesh("../model/bow.obj", programID,0);
    lantern1 = getMesh("../model/lantern1.obj", programID,0);
    lantern3 = getMesh("../model/lantern3.obj", programID,0); 
    circle = getMesh("../model/circle.obj", programID,0);
    //slime = getMesh("../model/slime.obj",programID,0);

    //---------------------phy(object)-------------------------//
    phy_sphere = getMesh("../model/solidsphere.obj", programID,1);
    phy_cube = getMesh("../model/cube.obj", programID,1);
    phy_cloud = getMesh("../model/cloud1.obj", programID,1); 
    
    //---------------------(object)-------------------------//
    slime = getMesh("../model/slime.obj",programID,2);
}   
void GraphicObj::drawByID(int graID, unsigned int programID)
{
    switch(graID){
        case YU_GRAPHICS_SPHERE:
            phy_sphere->draw(programID );
            break;
        case YU_GRAPHICS_CUBE:
            phy_cube->draw(programID);
            break;
        case YU_GRAPHICS_CLOUD:
            //cout << "draw cloud\n";
            phy_cloud -> draw(programID);
            break;
        case YU_GRAPHICS_SLIME:
            slime->draw(programID);
            break;
        default:
            cout << "GraphicObj::drawByID(int graID, unsigned int programID) has wrong!!!\n"; 
            break;
    }
}
const glm::mat3 &GraphicObj::getIByID(int graID)
{
    switch(graID){
        case YU_GRAPHICS_CLOUD:
            return phy_cloud->getI();
            break;
        default:
            cout << "GraphicObj::getIByID has wrong!!! in GraphicObj.cpp\n";
            break;
    }    
    return glm::mat3();
}
const vector<glm::vec3> &GraphicObj::getVerticesByID(int graID)
{
    switch(graID){
        case YU_GRAPHICS_CUBE:
            return phy_cube->getVertices();
        case YU_GRAPHICS_SPHERE:
            return phy_sphere->getVertices();
        case YU_GRAPHICS_CLOUD:
            return phy_cloud->getVertices();
        case YU_GRAPHICS_SLIME:
            return slime->getVertices();
        default:
            cout << "GraphicObj::getVerticesByID has wrong!!! in GraphicObj.cpp\n";
            break;
    }    
    // TODO: insert return statement here
}
mesh *GraphicObj::getMesh(string fname, unsigned int programID, int type)
{
    
    fstream file(fname.c_str(),std::fstream::in | std::fstream::ate); //ate -> 移到最後面
    int sz = file.tellg();  //回傳當前指針位置( ate在尾巴 -> 檔案大小 )
    file.seekg(0); //移到頭
    vector<float>tp;
    string s;
    float x, y, z;
    vector<vertex> v(1), vt(1), vn(1);
    while (file >> s) {
        //cout << fname << "\n";
        if (s == "v") {
            file >> x >> y >> z;
            v.push_back(vertex(x, y, z));
        }
        else if (s == "vt") {
            file >> x >> y;
            vt.push_back(vertex(x, y));
        }
        else if (s == "vn") {
            file >> x >> y >> z;
            vn.push_back(vertex(x, y, z));
        }
        else if (s == "f") {
            char x;
            int a, b, c;
            for (int i = 0; i < 3; i++) {
                file >> a >> x >> b >> x >> c;
                if (a > v.size()) {
                    cout << fname << "\n";
                    cout << a << "\n";
                    
                }
                tp.push_back(v[a].a); tp.push_back(v[a].b); tp.push_back(v[a].c);
                tp.push_back(vt[b].a); tp.push_back(vt[b].b);
                tp.push_back(vn[c].a); tp.push_back(vn[c].b); tp.push_back(vn[c].c);
            }
        }
    }
    file.close();
    mesh* m = new mesh(programID, tp,type);
    cout <<fname << " " <<tp.size() << "\n";
    
    return m;
}


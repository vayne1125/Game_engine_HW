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
    
    solidsphere = getMesh("../model/solidsphere.obj", programID);
    cube = getMesh("../model/cube.obj", programID);
    cylinder = getMesh("../model/cylinder.obj", programID);
    solidtorus_5_025 = getMesh("../model/solidtorus_0.5_0.025.obj", programID);
    fountain_base = getMesh("../model/fountain_base.obj", programID);
    fountain_water = getMesh("../model/fountain_water.obj", programID);
    blacksmith = getMesh("../model/Blacksmith.obj", programID);
    square = getMesh("../model/square.obj", programID);
    vendor_base = getMesh("../model/vendor_base.obj", programID);
    vendor_col = getMesh("../model/vendor_col.obj", programID);
    vendor_roof = getMesh("../model/vendor_roof.obj", programID);
    vendor_desk = getMesh("../model/vendor_desk.obj", programID);
    tree_round_up = getMesh("../model/tree_round_up.obj", programID);
    tree_round_btn = getMesh("../model/tree_round_btn.obj", programID);
    tree_conical_up = getMesh("../model/tree_conical_up.obj", programID);
    tree_conical_btn = getMesh("../model/tree_conical_btn.obj", programID);
    cloud1 = getMesh("../model/cloud1.obj", programID);
    vendor_crate = getMesh("../model/vendor_crate.obj", programID);
    apple = getMesh("../model/apple.obj", programID);
    cheese = getMesh("../model/cheese.obj", programID);
    bread2 = getMesh("../model/bread2.obj", programID);
    plate = getMesh("../model/plate.obj", programID);
    sword = getMesh("../model/sword.obj", programID);
    bow = getMesh("../model/bow.obj", programID);
    lantern1 = getMesh("../model/lantern1.obj", programID);
    //lantern2 = getMesh("../model/lantern2.obj", programID);
    lantern3 = getMesh("../model/lantern3.obj", programID); 
    
}
mesh* GraphicObj::getMesh(string fname, unsigned int programID)
{
    
    fstream file(fname.c_str(),std::fstream::in | std::fstream::ate); //ate -> 移到最後面
    int sz = file.tellg();  //回傳當前指針位置( ate在尾巴 -> 檔案大小 )
    file.seekg(0); //移到頭
    //string data;
    //data.resize(sz);
    //file.read((char*)data.data(), sz);
    //file.close();
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
    mesh* m = new mesh(programID, tp);
    cout <<fname << " " <<tp.size() << "\n";
    
    return m;
}


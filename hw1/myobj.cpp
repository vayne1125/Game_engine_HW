#define _CRT_SECURE_NO_WARNINGS
#include "myobj.h"
struct vertex {
    float a, b, c;
    vertex(float a_ = 0, float b_ = 0, float c_ = 0) {
        a = a_;
        b = b_;
        c = c_;
    }
};
myobj::myobj(unsigned int programID)
{
    solidsphere = getMesh(".\\model\\solidsphere.obj", programID);
    cube = getMesh(".\\model\\cube.obj", programID);
    cylinder = getMesh(".\\model\\cylinder.obj", programID);
    solidtorus_5_025 = getMesh(".\\model\\solidtorus_0.5_0.025.obj", programID);
}
mesh* myobj::getMesh(string fname, unsigned int programID)
{
    freopen(fname.c_str(), "r", stdin);
    vector<float>tp;
    string s;
    float x, y, z;
    vector<vertex> v(1), vt(1), vn(1);
    while (cin >> s) {
        cout << fname << "\n";
        if (s == "v") {
            cin >> x >> y >> z;
            v.push_back(vertex(x, y, z));
        }
        else if (s == "vt") {
            cin >> x >> y;
            vt.push_back(vertex(x, y));
        }
        else if (s == "vn") {
            cin >> x >> y >> z;
            vn.push_back(vertex(x, y, z));
        }
        else if (s == "f") {
            char x;
            int a, b, c;
            for (int i = 0; i < 9; i += 3) {
                cin >> a >> x >> b >> x >> c;
                tp.push_back(v[a].a); tp.push_back(v[a].b); tp.push_back(v[a].c);
                tp.push_back(vt[b].a); tp.push_back(vt[b].b);
                tp.push_back(vn[c].a); tp.push_back(vn[c].b); tp.push_back(vn[c].c);
            }
        }
    }
    fclose(stdin);
    cin.clear();
    mesh* m = new mesh(programID, tp);
    //cout << tp.size() << "\n";
    return m;
}

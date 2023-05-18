#include "Object.h"
Object::Object(std::string GraphicObjName, std::string PhyObjName, float r, float m, float k)
{
    if(PhyObjName == "sphere" || PhyObjName == "Sphere"){
        phyobj = new Sphere(r,m,k);
    }
}

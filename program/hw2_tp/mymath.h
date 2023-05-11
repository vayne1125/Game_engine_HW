#pragma once
struct vec3{
    union{
        struct{
            float x,y,z;
        };
        struct{
            float r,g,b;
        };
        float v[3];
    };
    float dot(const vec3& r)const{
        return (v[0]*r.v[0] + v[1]*r.v[1] + v[2]*r.v[2]);
    }
    vec3& operator-=(const vec3 r){
        for(int i=0;i<3;i++) v[i] -= r.v[i];
        return *this;
    }
    vec3 operator-(const vec3 r)const{
        vec3 rt;
        for(int i=0;i<3;i++) rt.v[i] = v[i] - r.v[i];
        return rt;
    }
    vec3 cross(const vec3& r)const{
        vec3 rt;
        rt.v[0] = v[1]*r.v[2] - v[2]*r.v[1]; 
        rt.v[1] = - v[0]*r.v[2] + v[2]*r.v[0];
        rt.v[2] = v[0]*r.v[1] - v[1]*r.v[0];
        return rt;
    }
    vec3& operator*=(const int r){
        for(int i=0;i<3;i++) v[i] *= r;
        return *this;
    }
    vec3 operator*(const int r)const{
        vec3 rt;
        for(int i=0;i<3;i++) rt.v[i] = v[i] * r;
        return rt;
    }
};
class mat3{
public:
    union{
        float m33[3][3];
        float m9[9];
        vec3 vec[3];
    };
    mat3(){};
    mat3(float dig){
        for(int i=0;i<9;i++){
            m9[i] = (i%4 == 0)?dig:0;
        }
    };
    vec3 operator*(const vec3& r)const{
        vec3 rt;
        rt.v[0] = vec[0].dot(r);
        rt.v[1] = vec[1].dot(r);
        rt.v[2] = vec[2].dot(r);
        return rt;
    }
private:

};
struct quaternions{
    union{
        float v[4];
        struct{
            float w,x,y,z;
        };
    };
    quaternions operator+(const quaternions r)const{
        quaternions rt;
        for(int i=0;i<4;i++) rt.v[i] = v[i] + r.v[i];
        return rt;
    }    
};


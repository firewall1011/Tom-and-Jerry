#ifndef _VECTOR2_
#define _VECTOR2_

#include <iostream>
#include <math.h>

class Vector2{
    public:
    double x, y;
    
    //Constructors
    Vector2(){
        x = y = 0;
    }

    Vector2(double nx, double ny){
        x = nx;
        y = ny;
    }

    //Operators
    Vector2 operator+=(Vector2 v){ return *this = Vector2(x+v.x, y+v.y); }
    Vector2 operator+=(double d) { return *this = Vector2(x+d, y+d); }

    Vector2 operator+(Vector2 v){ return Vector2(x+v.x, y+v.y); }
    Vector2 operator+(double d) { return Vector2(x+d, y+d); }

    Vector2 operator-=(Vector2 v){ return *this = Vector2(x-v.x, y-v.y); }
    Vector2 operator-=(double d) { return *this = Vector2(x-d, y-d); }

    Vector2 operator-(Vector2 v){ return Vector2(x-v.x, y-v.y); }
    Vector2 operator-(double d) { return Vector2(x-d, y-d); }

    Vector2 operator*=(double d){ return *this = Vector2(x*d, y*d); }
    Vector2 operator*(double d) { return Vector2(x*d, y*d); }

    Vector2 operator/=(double d){ return *this = Vector2(x/d, y/d); }
    Vector2 operator/(double d) { return Vector2(x/d, y/d); }

    friend std::ostream& operator<<(std::ostream& os, const Vector2 v){ os << "(" << v.x << ", "<< v.y << ")"; return os;}

    //Methods
    Vector2 normal(){ return Vector2(x/mag(), y/mag()); }
    Vector2 normalize(){ Vector2 v = normal(); x = v.x; y = v.y; }

    double mag(){ return sqrt(x*x + y*y); }
    double distance(Vector2 v){ return (v - *this).mag(); }

};

#endif
#ifndef POINTLIGHT_H 
#define POINTLIGHT_H

#include <string>
#include <iostream>
using namespace std;


#include "Direction.h"
#include "Point.h"
#include "light.h"

class PointLight : public Light { 

    public:
        PointLight(Point center, Point power) : Light(center,power) {
        }
        
        bool type(Point x) override {
            //cout << "PointLight" << endl;
            //cout << "point: " << endl;
            //x.print();
            return false;
        }
};

#endif
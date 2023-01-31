#ifndef LIGHT_H 
#define LIGHT_H

#include <string>
#include <iostream>
using namespace std;


#include "Direction.h"
#include "Point.h"

class Light { 
    private:
        Point c = Point(0,0,0);
        Point p = Point(0,0,0); // Color de la luz

    public:
        Light(Point center, Point power) {
                c = center;
                p = power;
        }

        Point getC(){
            return c;
        }

        Point getP(){
            return p;
        } 

        virtual bool type(Point x) {
            return false;
        }

        virtual bool sombras() {
            return false;
        }

        
};

#endif
#ifndef LIGHTAREA_H 
#define LIGHTAREA_H

#include <string>
#include <iostream>
#include <cmath>
using namespace std;

#include "light.h"
#include "Direction.h"
#include "Point.h"
#include "plano.h"

class AreaLight : public Light{ 
    private:
        Plano c = Plano(Direction(0,0,0),1,Point(1,1,1),Point(1,1,1),Point(1,1,1),1);

    public:
        AreaLight(Plano plano, Point power) : Light(plano.getP(), power) {
            c = plano;
        }

        bool type(Point x) override {
            Direction n = c.getn(x);
            
            float y = abs(n.x()*x.x()+n.y()*x.y()+n.z()*x.z()+c.getD());
            //cout << y << endl;
            if ( y < 0.00001) {
                //cout << "Encontré luz" << endl;
                return true;
            }
            return false;
        }

        bool sombras() override {
            return true;
        }
        
};

#endif
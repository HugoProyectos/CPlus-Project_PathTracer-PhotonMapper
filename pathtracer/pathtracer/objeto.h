#pragma once

#ifndef OBJETO_H 
#define OBJETO_H

#include "ray.h"

const float Pi = 3.14159265358979323846;

 class objeto {
    protected:
        Point kd = Point(0,0,0), ks = Point(0,0,0), kt = Point(0,0,0);
        float eta;
    public:
        objeto(Point _kd, Point _ks, Point _kt, float _eta) {
            kd = _kd;
            ks = _ks;
            kt = _kt;
            eta = _eta;
        }

        virtual float intersection(Ray& r, bool& error) const { 
            cout << "No funciona inter" << endl;
        }

        virtual Direction getN(Ray r, Point x) {
            cout << "No funciona getN" << endl;
        }

        virtual Direction getn(Point x) {
            //cout << "No funciona getn" << endl;
        }

        float getr(){return kd.x();}
        float getg(){return kd.y();}
        float getb(){return kd.z();}
        Point getKd(){return kd;}

        Point getKs(){return ks;}
        Point getKt(){return kt;}
        float getEta(){return eta;}

        virtual void string() const
        {
            cout << "Soy objeto." << endl;
        }
};

#endif
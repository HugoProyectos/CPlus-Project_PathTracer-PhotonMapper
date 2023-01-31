#ifndef PLANE_H 
#define PLANE_H

#include <string>
#include <iostream>
using namespace std;


#include "Direction.h"
#include "Point.h"
#include "Ray.h"
#include "function.h"
#include "objeto.h"



class Plano : public objeto  { 
    private:
        Point p =  Point(0,0,0);
        Direction n =  Direction(0,0,0); //Es la normal al plano
        float d = 0;

        Point OrigenCoor = Point(0,0,0);
    public:
        Plano(Direction _n, float _d, Point _kd, Point _ks, Point _kt, float _eta) : objeto(_kd, _ks, _kt, _eta)  {
            n = _n;
            d = _d;
        }

        Point getP () {
            return p;
        }

        Direction getn(Point x) override{
            return n.normal();
        }

        Direction getN (Ray r, Point x) override {
            //return n.normal();
            float angulo = dot(n.normal(), r.dir());
            if (angulo < 0) {
                return n.normal();
            } else {
                return Direction(0,0,0) - n.normal();
            }
        }

        float getD () {
            return d;
        }
        
        float intersection(Ray& r, bool& error) const override {
            error = true;
            float denominador = dot(r.dir(),n);
            if(denominador == 0){
                error = true;
                return 0;
            } else {
                //(c+o*n)
                Direction o_dir = r.origen() - OrigenCoor; 
                float numerador = d + dot(o_dir, n);
                
                float resultado = -numerador/denominador;
                
                if(resultado >= 0.0001){
                    error = false;
                    //cout << resultado << endl;
                    return resultado;
                } else{
                    error = true;
                    return 0;
                }

            }
            return 0;
        }

/*
            // t = -(c - o*n) / d*n
            // o. origen rayo
            // n. normal al plano
            // d. dir. del rayo
            // c. distancia eje de coordenadas parametro def. el plano(siguiendo la normal) 

            //Un punto es el origen de coordenadas mas un vector hasta el
            //Ec a resolver
            //t = - (c+o*n)/(d*n)

            //c => Se genera con una resta y un producto escalar.
           /* error = false;
            if((r.dir()*n).abs() == 0) {
                //No colisiona
                error = true;
                return 0;
            } else {
                //Hay colision
                Direction oD = Direction(r.origen().x(), r.origen().y(),r.origen().z());
                return -(d+(n*oD).abs() / (r.dir()*n).abs());
            }

            //t = - (c+o*n)/(d*n)
            //d*n
*/


};


#endif
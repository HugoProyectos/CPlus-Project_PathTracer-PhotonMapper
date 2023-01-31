#ifndef RAY_H 
#define RAY_H

#include <string>
#include <iostream>
using namespace std;



#include "Direction.h"
#include "Point.h"

class Ray { 
    private:
        Point p = Point(0,0,0);
        Direction d = Direction(0,0,0);

    public:
        Ray(Point o, Direction dir) {
                p = o;
                d = dir;
        } 

        Point origen() {
            return p;
        }

        Direction dir() {
            return d;
        }

        // Devuelve el pto. a distancia t.
        Point rayPoint(float t){
            Direction prueba = d*t;
            return p + prueba;
        }

        // Devuelve la distancia de viaje entre dos ciudades.
        float Distance(Point c1){
            return (c1 - p).abs();
        }

        // Devuelve la normal a la superficie en la que intersecciona.
        // Dado dos ptos: Ciudad de origen y ciudad destino.
        void Normal(Point c1, Point c2){
            /*if (colision(c1,c2)) {
                // Si hay intersección, devuelve la normal de dicho pto.
            } else {
                // No intersecciona.
            }*/
        }


        
};

#endif
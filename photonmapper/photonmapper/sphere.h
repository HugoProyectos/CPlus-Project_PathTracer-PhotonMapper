#ifndef SPHERE_H 
#define SPHERE_H

#include <string>
#include <iostream>
using namespace std;

#include <cmath>
#include "Direction.h"
#include "Point.h"
#include "Ray.h"
#include "function.h"
#include "objeto.h"

class Sphere : public objeto  {
private:
    Point center = Point(0,0,0);
    Point city = Point(0,0,0);  //Radio
    Direction axis = Direction(0,0,0);
    float radix = 0;

public:
    Sphere(Point p, Point c, Direction d, Point _kd, Point _ks, Point _kt, float _eta) : objeto(_kd, _ks, _kt, _eta) {
        center = p;
        city = c;
        axis = d;
    }

    Sphere(Point p, float rad, Point _kd, Point _ks, Point _kt, float _eta) : objeto(_kd, _ks, _kt, _eta) {
        center = p;
        radix = rad;
    }

    Point getCenter(){return center;}
    Direction getAxis(){return axis;}
    float getRadio(){return axis.abs()/2;}

    Direction getn (Point x) override{
        return ((x) - (center)).normal();
    }
    Direction getN(Ray r, Point x) override {
        Direction nor =  ((x) - (center)).normal();
        float angulo = dot(nor, r.dir());
            if (angulo < 0) {
                return nor;
            } else {
                return Direction(0,0,0) - nor;
        }
        return ((x) - (center)).normal();
    }


    //Para definir una cuidad, inclinación respecto al centro -> theta && Inclinación respecto city ->
    /*
        nos dan, una esfere, azimuth e inclinacion sacar la cuidad
        obtener vector con inclinacion y azimuth rotando el eje de la esfera, apuntas a la city
        city = centro + vector v
    */
    



    /*
    Respecto a las colisiones, efectivamente hay que calcular la normal en la superficie del planeta, y 
    entonces calcular el ángulo respecto a la dirección. No es necesario aplicar el cambio de base, si ya lo habéis aplicado.
    Me explico. La normal a una superficie, en este caso la ciudad, en su espacio local es siempre 
    el vector unitario 0*i + 0*j + 1*z, pero tendrá otro valor en las coordenadas del planeta 
    (recordad el cálculo del vector a partir de la azimuth y la inclinación), y otro distinto en las universales.
    */
    
    // Se suponen coordenadas universales.
    bool colision(Point c1, Point c2) {
        //Normal a la superficie
        Direction n = c1 - center;
        
        //Vector direccion viaje c1->c2
        Direction viaje = c2 - c1;

        //Hay que normalizar las direcciones
        viaje = viaje.normal();
        n = n.normal();

        //Calcular ángulo respecto a la direccion
        float angulo = dot(n,viaje);

        if(angulo <= 90*Pi/180) {
            //No colision
            return false;
        } else {
            //Colision
            return true;
        }
    }

    
        float intersection(Ray& r, bool& error) const override{ 
            //Implicit equation f(x,y,z) = (x - cx)² + (y - cy)² + (z - cz)² - r²
            // Si f(x,y,z) != 0; No está en la superficie de la esfera.
        
            //t^2 * r.d.abs()^2 + 2*t*r.d * (r.p - center()) + (r.p - center()).abs()^2 - radio()^2;
            float a = (r.dir().abs());
            
            a = a*a;
            Direction aux = r.origen() - center;
            float b = 2 * dot(r.dir(), aux);
            float c = aux.abs();
            c = c*c - radix*radix;
            

            //Funcion cuadrática
            
            /*Direction aux = r.origen() - center;
            float b = 2 * dot(r.dir(), aux);
            float c = (aux.abs(),2);*/

            error = false;
            float resultado = 0;
            if(2*a == 0 || ((pow(b,2) - (4*a*c)) < 0)) {
                //no hay colision
                error = true;
                return 0;
            }
            
            if(pow(b,2) - 4*a*c == 0){
                //Una colision
                cout << "UNA" << endl;
                float t = -b/2*a;
                
                //return t;
            } else{
                //Dos colisiones
                    //cout << "a: " << a << endl;
                    //cout << "b: " << b << endl;
                    //cout << "c: " << c <<endl;
                   // cout << " sqrt pow(b,2) - 4*a*c: " << sqrt(pow(b,2)- 4*a*c) <<endl;
                float t1 = (-b-sqrt(b*b- 4*a*c))/(2*a);
                //cout << "t1: " << t1 << endl;
                float t2 = (-b+sqrt(b*b- 4*a*c))/(2*a);
                //cout << "t2: " << t1 << endl;

                Point colision = r.rayPoint(t1);
                //cout << "DOS" << endl;

                //ajustar el resultado
                if (t1 < 0.0001) {
                    t1 = t2;
                } else if(t2 < 0.0001){t2 = t1;}
                if (r.Distance(r.rayPoint(t1)) < r.Distance(r.rayPoint(t2))) {
                    resultado =  t1;
                } else {resultado =  t2;}



                if(resultado >= 0.0001){
                    error = false;
                    //cout << resultado << endl;
                    return resultado;
                } else{
                    error = true;
                    return 0;
                }
            
            }
            //-b+-sqrt(b^2 - 4ac) /2a
            error = true;
            return 0;   
        }


};


#endif
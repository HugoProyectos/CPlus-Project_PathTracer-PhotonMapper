#pragma once

#ifndef CAMARA_H 
#define CAMARA_H

#include "Direction.h"
#include "Point.h"
#include "Ray.h"
#include "function.h"
#include "sphere.h"
#include "plano.h"
#include "light.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

// Rebotes del path_tracing
const int rebotes = 50;
int ndifusos = 0, nespeculares = 0, nrefracciones = 0, nabsorciones = 0;
bool entrada = false;

class Camara { 
    private:
        Point o =  Point(0,0,0);
        // Las sig. dir. definen la imagen del plano.
        Direction l =  Direction(0,0,0); 
        Direction u =  Direction(0,0,0);
        Direction f =  Direction(0,0,0);
        //Definimos el nº de pixeles.
        float width;
        float height;

    public:
        Camara(Point _o, Direction _l, Direction _u, Direction _f, float w, float h) {
            l = _l;
            u = _u;
            f = _f;
            o = _o;
            width = w;
            height = h;
        }

        // Devuelve la forma de tratar el siguiente evento
        //      1 - Difuso
        //      2 - Especular
        //      3 - Refractante
        //      4 - Absorcion
        Direction ruleta_rusa(objeto* obj, Direction wo, Point x, Direction normal, Point& a, bool& absorcion, Ray r, bool& especularBool) {
            // Generar probabilidad aleatoria
            float prob = (((double)rand())/((double)RAND_MAX));
            
            Point kd = obj->getKd(); 
            Point ks = obj->getKs(); 
            Point kt = obj->getKt();
            
            // Probabilidad de material difuso    
            float pd = kd.maximo();
            // Probabilidad de material especular
            float ps = ks.maximo();
            // Probabilidad de materia refractante
            float pt = kt.maximo();

            float ptotal = pd + ps + pt;
            float pDifuso = pd*0.9/ptotal; 
            float pEspecular = (pd+ps)*0.9/ptotal; 
            float pRefracta = (pd+ps+pt)*0.9/ptotal; 

            // Probabilidad de absorción
            if (kd.x() + ks.x() + kt.x() > 1) {
                cerr << "Error: kd + ks + kt > 1" << endl;
            }
            if (kd.y() + ks.y() + kt.y() > 1) {
                cerr << "Error: kd + ks + kt > 1" << endl;
            }
            if (kd.z() + ks.z() + kt.z() > 1) {
                cerr << "Error: kd + ks + kt > 1" << endl;
            }
            //float pabsorcion = 1.0 - (pd+ps);//+pt);

            //Direcciones perpendiculares a la normal
            Direction perpendicular1 = cross(normal, transformation(normal)).normal();
            Direction perpendicular2 = cross(normal, perpendicular1).normal();
            float theta;
            Direction wi = Direction(0,0,0);

            if(prob < pDifuso){
                //Se hace difuso
                especularBool = false;
                ndifusos++;
                wi = change_base(sample(theta), x, perpendicular1, perpendicular2, normal).normal();
                //Los terminos del coseno (Uniform solid angle sampling) -> multiplican por  2*cos(theta)*Pi? (Diapo 58 Montecarlo)
                float terminoCoseno = abs(dot(normal.normal(), wi.normal()));
                //terminocoseno = |n*wi|
                //kd * terminocoseno * 2 * cos(0)
                Point kd = Point(obj->getr(), obj->getg(), obj->getb());
                terminoCoseno = terminoCoseno * 2 * cos(theta) * 1/pd;
                a =  kd * terminoCoseno;
                //a.print();
                return wi;
                //return 1;
            } else if((prob < pEspecular) && prob >= pDifuso){
                especularBool = true;
                //Se hace reflection
                nespeculares++;
                //wi = change_base(especular(wo, normal), x, perpendicular1, perpendicular2, normal).normal();
                // Direction wi = wo - normal*2*dot(wo,normal);
                wi = especular(wo, normal).normal();
                //Los terminos del coseno (Uniform solid angle sampling)
                // fr * |n*wi| = 1
                //terminocoseno = |n*wi|
                //ks * terminocoseno * 2 * pi * cos(0)
                float probEspecular = (ps*0.9/ptotal);
                a = ks / probEspecular;

                /*
                    float fr = dot(normal.normal(), wi.normal());
                    float terminoCoseno = abs(fr);
                    Point brdf = Point(1/fr, 1/fr, 1/fr);
                    //Calcular theta dot(normal,wo)*wo.abs()*normal.abs()
                    //cout << dot(normal,-wo)/(wo.abs()*normal.abs()) << endl;
                    terminoCoseno = terminoCoseno * 2 * Pi * dot(normal,-wo)/(wo.abs()*normal.abs())  * 1/(ps*0.9/ptotal) * cos(dot(normal,wo)/(wo.abs()*normal.abs()));
                    a =  brdf * terminoCoseno;
                */
                return wi;
                //return 2;
            } else if((prob < pRefracta) && prob >= pEspecular){
                especularBool = true;
                nrefracciones++;
                //respecto al rayo
                Direction aux = obj->getN(r,x);
                Direction aux2 = obj->getn(x);
                //aux.print();
                //cout << "-----------" << endl;
                bool entrada = false;
                if(aux == aux2){
                    entrada = true;
                    //cout << "entrada" << endl;
                }
                else {
                    //cuando el rayo sale, la normal va hacia dentro,
                    //que es distinta de la que se obtiene al restar al centro
                    entrada = false;
                    //cout << "salida" << endl;
                }

                //ya sabemos si entramos o salimos
                float eta_o, eta_i, relacionEta;
                if(entrada){
                    eta_o = 1;
                    eta_i = obj->getEta();
                }
                else {
                    eta_o = obj->getEta();
                    eta_i = 1;
                }
                //relacion
                relacionEta = eta_o/eta_i;

                //Obtner en RADIANES el theta_o
                float theta_o = acos(dot(-wo, normal)/(wo.abs()*normal.abs()));
                //cout << sin(theta_o) << endl;
                //cout << (theta_o*180/3.14) << endl;

                //Ya tenemos el theta_o, la relaciond e snell->obtener theta_i
                float theta_i = sin(theta_o)*relacionEta;

                float probRefraccion = (pt*0.9/ptotal);
                a = kt / probRefraccion;

                float n = eta_o/eta_i;
                return wo.normal()*n + aux*(n*cos(theta_o) - sqrt((1 - (sin(pow(theta_i,2))) )) );

                /*
                Direction cb = change_base(-aux, x, perpendicular1, perpendicular2, normal).normal();

                wi = rotation(theta_i, X, cb);
                wi = rotation(theta_i, Y, wi);
                wi = rotation(theta_i, Z, wi);
                
                float probRefraccion = (pt*0.9/ptotal);
                a = kt / probRefraccion;
                
                return wi;
                */

            } else {
                //Absorber
                nabsorciones++;
                absorcion = true;
                return Direction(0,0,0);
                //return 4;
            }        
        }

        //Diffuse BRDF sample function
        //Esta función se llama en cada rebote [Path tracing]
        //Devuelve el RGB.
        Direction sample(float& theta) {
            //Uniform Solid Angle Sampling
            //Diapo 58 Montecarlo
            float num = (((double)rand())/((double)RAND_MAX));
            theta = acos(num); 

            num = (((double)rand())/((double)RAND_MAX));
            float phi = 2 * Pi * num;

            //Diapo 67 Montecarlo
            Direction wi = Direction(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
            
            return wi;
        }
        
        //Perfect Specular BRDF sample function
        Direction especular(Direction wo, Direction normal) {
            //wi = wr = wo -2*n(dot(wo,n))
            Direction wi = wo - normal*2*dot(wo,normal);
            return wi;
        }

        //Perfect Refraction BRDF sample function
        Direction refraccion(Direction wo, Direction normal, float eta_o, float eta_i) {
           
            //Comprobar dir incidente ¿?
            float aux = dot(wo.normal(),normal.normal());
            return wo * (eta_o/eta_i) - normal * (-aux + eta_i*aux);
        }

        //Path tracing (recursivo)
        //Path tracing == Lo(x,wo)
        Point path_tracing(Ray r, Light* l, int n, objeto* obj[], float nf) {
            // si hay más de N rebotes
            if (n <= 0) { return Point(0,0,0);}
            // intersectar rayo con geometría en el pto. x
            float distancia;
            float distanciaMin = INFINITY;
            objeto* objeto;
            bool error = false;
            bool intersecta = false;
            for(int index = 0; index < nf; index++ )
            {
                distancia = obj[index]->intersection(r,error);
                if(!error && (distancia < distanciaMin)){
                    distanciaMin = distancia;
                    objeto = obj[index];
                    intersecta = true;
                }
            }

            if(!intersecta) { //PARAMOS
                return Point(0,0,0);
            }
            else {
                Direction aux = r.dir()*distanciaMin;
                Point x = r.origen() + aux;
                if (l->type(x)){
                    return l->getP();
                } 
                //Luz de sombra
                bool sombra = false;
                bool error;
                //Punto donde intersecta el rayo
                Direction interseccionLuzDir = (l->getC() - x);
                float interseccionLuzDist = interseccionLuzDir.abs();
                Ray interseccionLuzRay = Ray(x,interseccionLuzDir.normal());
                for(int index = 0; index < nf; index++ )
                {
                    distancia = obj[index]->intersection(interseccionLuzRay,error);
                    if(!error && (distancia < interseccionLuzDist)){
                        //cout <<"distancia: " << distancia << endl;
                        sombra = true;
                        break;
                    }
                }

                //Quitar sombras
                //sombra = false;

                Direction normal = objeto->getN(r,x).normal();
                //normal.print();
                Point color(0,0,0);
                Point a = Point(0,0,0);
                bool absorcion = false;
                bool especularBool = false;
                Direction wi = ruleta_rusa(objeto,aux,x,normal,a,absorcion,r,especularBool);
                
                if(!sombra && !l->sombras() && !especularBool) {  
                    //Luz directa
                    Point L = directa(l->getP(), l->getC(), x, normal, objeto);
                    
                    //Multiplicar por término de coseno
                    color = Point(L.x()* objeto->getr(), L.y()* objeto->getg(), L.z()* objeto->getb());
                }
                
                
                if (absorcion) {
                    return Point(0,0,0);
                }
                //Lanzar el nuevo rayo
                Ray nuevoRayo = Ray(x, wi);
/*                float theta = 0;
                //Los terminos del coseno (Uniform solid angle sampling) -> multiplican por  2*cos(theta)*Pi? (Diapo 58 Montecarlo)
                float terminoCoseno = abs(dot(normal.normal(), wi.normal()));
                //terminocoseno = |n*wi|
                //kd * terminocoseno * 2
                Point kd = Point(objeto->getr()/255, objeto->getg()/255, objeto->getb()/255);
                terminoCoseno = terminoCoseno * 2 * cos(theta);
                a =  kd * terminoCoseno;
*/              //a.print();
                //Ignorar el primer rebote
                if(especularBool) {
                    return path_tracing(nuevoRayo, l, n-1, obj, nf)*a;
                } else {
                    return path_tracing(nuevoRayo, l, n-1, obj, nf)*a + color;
                }
                
            }
        }

        //Diffuse BRDF evaluation function
        //f_r(x, w_i, w_o)
        Point difusa(objeto *obj){
            Point kd = Point(obj->getr(), obj->getg(), obj->getb());
            float pi = Pi;
            return kd/pi;
        }

        //next-event estimation
        //Point power, Point c, Point x, Direction normal
        Point directa(Point power, Point c, Point x, Direction normal, objeto* obj){

            Direction distLuz = (c - x);
            float distLuzMod = (c - x).abs();
            float distLuzMod2 = pow(distLuzMod,2);

            Point primeraParte = (power/distLuzMod2);

            Point segundaParte = Point(0,0,0);
            float segundaAux = abs(dot(normal, (distLuz/distLuzMod)));
            segundaParte = difusa(obj) * segundaAux;
            Point L = primeraParte * segundaParte; 
            return L;
        }

        // n es el número de rayos
        void pixeles(int n, objeto *objetos[], Light *luces[], int nf, int nl, string nombre) {
            //El centro del plano sera el o + F
            Point c = o + f;
            Point x00 = c + u + l;
            //cout << "x00: ";
            //x00.print();
            Point x01 = c + u - l;
            //cout << "x01: ";
            //x01.print();
            Point x10 = c - u + l;
            //cout << "x10: ";
            //x10.print();
            Point x11 = c - u - l;
            //cout << "x11: ";
            //x11.print();

            ndifusos = 0, nespeculares = 0, nrefracciones = 0, nabsorciones = 0;
            
            //Dimensiones de un pixel
            float t1 = ((x01 - x00).abs()) / width;
            //cout << "ti: " << t1 << endl;
            float t2 = ((x10 - x00).abs()) / height;
            //cout << "t2: " << t2 << endl;
        
            //Calculamos el centro del pixel
            float dist_centro_x = t1/2;
            //cout << "dist_centro_x: " << dist_centro_x << endl;
            float dist_centro_y = t2/2;
            //cout << "dist_centro_y: " << dist_centro_y << endl;
            Point pixel = Point(0,0,0);

            //Abrir fichero resultado
            ofstream file;
            file.open(nombre + ".ppm");
            if(file.is_open()) {     
                file << "P3" << endl << "#MAX=1" << endl << "#resultado.ppm" << endl << height << " " << width << endl << "255" << endl;
                int contar = 0;
                for(int i = 0; i < height; i++){
                    for(int j = 0; j < width; j++){
                        contar++;
                        float rCol = 0;
                        float gCol = 0; 
                        float bCol = 0;

                        float mediaR = 0;
                        float mediaG = 0; 
                        float mediaB = 0;

                        for(int m = 0; m < nl; m++){
                            //Calcular el centro/pixel?k
                            //pixel = Point{((t1 * j) + dist_centro_x),-((t2 * i) + dist_centro_y),0} + x00;
                            
                            for(int k = 0; k < n; k++) {
                                float p1 = ((int)((((double)rand()) / ((double)RAND_MAX)) * 10000000) % (int)(t1 * 10000000)) / 10000000.0;
                                float p2 = ((int)((((double)rand()) / ((double)RAND_MAX)) * 10000000) % (int)(t2 * 10000000)) / 10000000.0;

                                
                                pixel = Point{((t1 * j) + p1),-((t2 * i) + p2),0} + x00;
                                //pixel.print();

                                float distancia = 0;
                                Direction dirAux = pixel - o;
                                Ray r = Ray(o, dirAux);   

                                Point power = Point(1,1,1);
                                //tendriamos que hacer un sumatorio de las luces de cada fuente de luz
                                Point luz = path_tracing(r, luces[m], 4, objetos, nf);
                            
                                rCol = luz.x();
                                gCol = luz.y();
                                bCol = luz.z();
    
                                mediaR = mediaR + rCol;
                                mediaG = mediaG + gCol;
                                mediaB = mediaB + bCol;

                            }
                        }

                        //Escribir el color 
                        file << mediaR*255/(float)n << " " << mediaG*255/(float)n  << " "  << mediaB*255/(float)n  << "     ";
                        
                    }
                file << endl;
                }
                file.close();
                // cout << "CONTAR: " << contar << endl;
                // cout << "Rayos difusos: " << ndifusos << endl;
                // cout << "Rayos especulares: " << nespeculares << endl;
                // cout << "Rayos refractantes: " << nrefracciones << endl;
                // cout << "Absorciones: " << nabsorciones << endl;
            } else {
                cerr << "No se ha podido abrir el fichero." << endl;
            }
        }
};


#endif
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
#include "kd_tree_example.cpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <list>

// Rebotes del path_tracing
const int rebotes = 50;
int ndifusos = 0, nespeculares = 0, nrefracciones = 0, nabsorciones = 0;
bool entrada = false;
// Caminos del photon_mapping
const int MAX_RANDOM_WALKS = 3;

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
                wi = especular(wo, normal).normal();
                //wi.print();
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


            } else {
                //Absorber
                especularBool = false;
                nabsorciones++;
                absorcion = true;
                return Direction(0,0,0);
                //return 4;
            }        
        }

        //Uniform solid angle sampling esfera
        Direction sampleSphere(float& theta) {
            //Uniform Solid Angle Sampling
            //Diapo 58 Montecarlo
            float num = (((double)rand())/((double)RAND_MAX));
            theta = acos(2 * num - 1); 

            num = (((double)rand())/((double)RAND_MAX));
            float phi = 2 * Pi * num;

            //Diapo 67 Montecarlo
            Direction wi = Direction(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
            
            return wi;
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
        
        //Diffuse BRDF evaluation function
        //f_r(x, w_i, w_o)
        Point difusa(objeto *obj){
            Point kd = Point(obj->getr(), obj->getg(), obj->getb());
            float pi = Pi;
            return kd/pi;
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
            float aux = dot(wo,normal);
            return wo * (eta_o/eta_i) - normal * (-aux + eta_i*aux);
        }

        //Devuelve la distancia a la que intersecta un rayo respecto a todos los objetos de la escena
        objeto* intersecta(int nf, objeto* obj[], Ray r, float &dist){
            float distanciaMin = INFINITY;
            float distancia = INFINITY;
            bool intersecta = false, error;
            objeto* objeto;
            for(int index = 0; index < nf; index++ )
            {
                distancia = obj[index]->intersection(r,error);
                if(!error && (distancia < distanciaMin)){
                    distanciaMin = distancia;
                    objeto = obj[index];
                    intersecta = true;
                }
            }

            if(intersecta){
                dist = distanciaMin;
                return objeto;
            } 
            else{ 
                dist = -1;
                return objeto;
            }
        }

        PhotonMap photon_mapping(float nf, objeto* obj[], int nl, Light *luces[], int fotonesTotales, int metodo){
            
            //lista con los photones
            list<Photon> listaPhoton;
            //kd-tree con los photones

            //Obtenemos el total de potencia de luz de la escena
            Point totalPowerPoint = Point(0,0,0);
            for(int i = 0; i < nl; i++){
                Point lightPower = luces[i]->getP();
                totalPowerPoint = totalPowerPoint + lightPower;
            }
            float totalPower = totalPowerPoint.suma();

            //El numero de phtones de la fuente de luz x => N*(px/ptotal)
            
            for(int i = 0; i < nl; i++){ //Para cada fuente de luz
                //Total de power de esa fuente de luz
                float fuentePower = luces[i]->getP().suma(); 
                //obtener el número de fotones corresponden a esta fuente de luz
                int nFotones = fotonesTotales*(fuentePower/totalPower); 

                Point energy = Point (0,0,0);
                energy = luces[i]->getP();
                float S = nFotones;
                
                int j = 0;
                while(j < nFotones){ //Para el número de fotones de la fuente de luz
                    //Lanzar rayos desde la fuente de luz con uniform solid angle sampling
                    //Comporbar N rebotes
                    //Para cada interseccion guardar el photon(position, direction, flux) correspondiente 
                    //----------------------------------------

                    //Light* luz, int nf,objeto* obj[], float S, Point energy, list<Photon> listaPhoton, int nwalks)
                    float theta = 0;
                    Direction dir = sampleSphere(theta).normal(); //Uniform solid angle sampling
                    //Crear el rayo
                    Ray ray = Ray(luces[i]->getC(), dir); //Punto de origen la fuente de luz y direccion aleatoria
                    scatter(ray, luces[i],nf,obj,nFotones,energy, listaPhoton, MAX_RANDOM_WALKS, nFotones, j, metodo);
                } //Fin for phtontes por luz
            } //Fin recorrer luces
        
            //Tenemos en una lista el total de fotones de la escena
            //creamos con esa lista el photon map
            //PhotonMap mapaPhoton = PhotonMap(listaPhoton, PhotonAxisPosition());
            list<Photon>::iterator myitr;
            
            
            // for (myitr = listaPhoton.begin(); myitr != listaPhoton.end(); ++myitr){
            //         cout << "Dir: "; myitr->direction().print();
            //         cout << "Pos: "; myitr->position().print();
            //         cout << "Flux: ";myitr->flux().print();
            //         cout << "------------------" << endl;
            //     }
            
            PhotonMap mapaPhoton = generation_of_photon_map(&listaPhoton);

            //Devolvemos el mapa de fotones de la escena, fin primer paso photonMapping
            return mapaPhoton;

        }

        Point scatter(Ray ray, Light* luz, int nf,objeto* obj[], float S, Point energy, list<Photon> &listaPhoton, int nwalks, int maxFotones, int &nFotones, int metodo){
            if (nwalks <= 0) {
                return Point(0,0,0);
            }
            if(nFotones >= maxFotones){
                return Point(0,0,0);
            }
            //Obtener direccion aleatoria para el rayo
            //float theta = 0;
            //Direction dir = sample(theta); //Uniform solid angle sampling
            //Crear el rayo
            //Ray ray = Ray(luz->getP(), dir); //Punto de origen la fuente de luz y direccion aleatoria
            
            //Comprobar intersecciones, nos quedamos con la mas cercana
            float distancia = INFINITY;
            float distanciaMin = INFINITY;
            objeto* objeto;
            bool error = false;
            bool intersecta = false;
            
            for(int index = 0; index < nf; index++ )
            {
                distancia = obj[index]->intersection(ray,error);
                if(!error && (distancia < distanciaMin)){
                    distanciaMin = distancia;
                    objeto = obj[index];
                    intersecta = true;
                }
            }

            if (!intersecta) { // Absorcion
                return Point(0,0,0);
            }

            Direction aux = ray.dir()*distanciaMin;
            Point x = ray.origen() + aux;
            Direction normal = objeto->getN(ray,x).normal();
            Point a(0,0,0);
            bool absorcion = false, especular = false;
            //RULETA RUSA
            Direction dir = ruleta_rusa(objeto,ray.dir(),x,normal,a, absorcion, ray, especular);

            //Ya tenemos el objeto mas cercano con el que intersecta el rayo, la distancia, y direction
            //Para construir el photon necesitamos direction, position y flux
            //Position
            Direction directionAux = ray.dir()*distanciaMin;
            Point position = ray.origen() + directionAux;
            //Direction 
            Direction direction = ray.dir();
            
            Point flux = Point(0,0,0);
            
            //Si es el priemr rebote, guardar la luz
            //Si es difuso, guardar el color anterior
            //Si es especular, cambiar la dirección, no gurar hoton, no entrar aqui
            if (nwalks == MAX_RANDOM_WALKS) {
                //FLux------------------------------------------
                //Diapo 48 luces[i]
                // p1 + p2 + p3 + p4 + p5 + p6 = 4 * Pi * p0
                // Tiene el valor de la fuente, rebota teniendo en cuenta su color
                float aux = 4*Pi;
                flux = (energy*aux) / S;

            } else { // No es el primer rebote de la luz
                //dividir probabilidad evento * ks
                flux = energy * a;
            }
            //Nos llega una luz
            //Esa luz -> flux
            //Vemos que refleja el material
            //Mandar eso como nueva luz

            //Añadir este phton a la lista
            if( (metodo == 1 && !especular)|| ((metodo == 2  && MAX_RANDOM_WALKS != nwalks  && !especular))){
                //Creamos el photon
                Photon photon = Photon(position,direction,flux);
                listaPhoton.push_front(photon); 
                nFotones++;
            } else if(!especular){
                nFotones++;
            }
            
            //Radiancia, cambiar si difuso
            if(!especular)
                flux = objeto->getKd() * flux;
            
            if (absorcion)
                return Point(0,0,0);
                
            Ray rayNew = Ray(position, dir); //Punto desde colision y direccion aleatoria
           
            return scatter(rayNew, luz,nf,obj, S, flux, listaPhoton, nwalks-1,maxFotones,nFotones, metodo);

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


        Direction rayo(Point& position, objeto* objetos[], objeto*& obj,int nf, Direction directionAux, Point& a, bool& absorcion, Ray& r, bool& especular) {
            Direction dir = directionAux;
            float distancia = 0;
            Direction normal(0,0,0);
            Point flux = Point(0,0,0);
            //Comprobar especular
            ruleta_rusa(obj,r.dir(),position,normal,a, absorcion, r, especular);

            while (especular) {
                normal = obj->getN(r,position).normal();
                
                absorcion = false; especular = false;          
                dir = ruleta_rusa(obj,r.dir(),position,normal,a, absorcion, r, especular);
                
                if (!especular || absorcion) {
                    break;
                }
                
                r = Ray(position, dir);  
                    
                float distanciaMin2 = INFINITY;
                bool error = false;
                bool intersect2 = false;
                
                for(int index2 = 0; index2 < nf; index2++ )
                {
                    distancia = objetos[index2]->intersection(r,error);
                    if(!error && (distancia < distanciaMin2)){
                        distanciaMin2 = distancia;
                        obj = objetos[index2];
                        intersect2 = true;
                    }
                }
                if (!intersect2) { // Absorcion
                    break;
                }
                distancia = distanciaMin2;

                // directionAux = r.dir()*distancia;
                // position = r.origen() + directionAux;

                Direction dirAux = dir * distancia;
                position = position + dirAux;

                //Calcular NUEVA posicion donde intersecta
                /*directionAux = r.dir()*distancia;
                position = r.origen() + directionAux;

                normal = obj->getN(r,position).normal();              
                dir = ruleta_rusa(obj,r.dir(),position,normal,a, absorcion, r, especular);
                */

            }
                
            return dir;
        }

        void pixelesPhoton(int nPhotones, long numPhotonsNear, float radiusPhotons, objeto *objetos[], Light *luces[], int nf, int nl, string nombre, int metodo, int nRayos) {

            PhotonMap photonMap = photon_mapping(nf, objetos, nl, luces, nPhotones, metodo);

            //El centro del plano sera el o + F
            Point c = o + f;
            Point x00 = c + u + l;
            Point x01 = c + u - l;
            Point x10 = c - u + l;
            Point x11 = c - u - l;
            
            //Dimensiones de un pixel
            float t1 = ((x01 - x00).abs()) / width;
            float t2 = ((x10 - x00).abs()) / height;
        
            //Calculamos el centro del pixel
            float dist_centro_x = t1/2;
            float dist_centro_y = t2/2;
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
                        
                        Point luz = Point(0,0,0);
                        for(int indice = 0; indice < nRayos; indice++) {
                            //Calcular el centro/pixel?k
                            pixel = Point{((t1 * j) + dist_centro_x),-((t2 * i) + dist_centro_y),0} + x00;
                            
                            //Coger los fotones mas cercanos al punto donde intersecta el rayo
                            //float distancia = 0;
                            Direction dirAux = pixel - o;
                            Ray r = Ray(o, dirAux);   
                            // cout << "Ray: "<<endl;
                            // r.dir().print();

                            
                            //Comprobar intersecciones, nos quedamos con la mas cercana
                            float distancia = 0;
                            objeto* obj;
                            
                            obj = intersecta(nf, objetos, r, distancia);
                                

                            if(distancia == -1) { //No intersecta o se absorbe el rayo
                                file << "0" << " " << "0"  << " "  << "0"  << "     ";
                                break;      
                            }
                            
                            Direction directionAux = r.dir()*distancia;
                            Point position = r.origen() + directionAux;


                            //RULETA RUSA
                            // si material difuso -> normal
                            // si especular -> continuar camnino del rayo
                            
                            bool absorcion = false; 
                            bool especular = true;
                            Direction normal = obj->getN(r,position).normal();
                            Point a(0,0,0);
                            Direction dir = rayo(position,objetos, obj,nf,directionAux,a, absorcion, r, especular);   
    
                            //Direction dir = ruleta_rusa(obj,r.dir(),position,normal,a, absorcion, r, especular);
                            if(!especular && !absorcion){
                                auto nearPhotons = photonMap.nearest_neighbors(position,numPhotonsNear,radiusPhotons);
                                //vector<const Photon*> nearPhotons = search_nearest(photonMap, position, numPhotonsNear, radiusPhotons);
                                int i = 0;
                                // for(auto photon : nearPhotons)    {
                                //     cout << "-------------------------------------------------" <<endl; 
                                //     cout << "position: "; position.print();
                                //     cout << "nearPosiiton: "; photon->position().print();
                                //     cout << "flux: "; photon->flux().print();
                                //     cout << "i: " << i << endl; i++;
                                // }
                                //cout <<  << endl;
                                

                                //Tenemos los fotones cercanos
                                //Kernel density estimation fr(x, w_p, w_o)flux_p/(Pi*r^2)
                                //Kernel density estimation fr(x, direction_photon, dir_entrada)flux/(Pi*r^2)
                                
                                float pi = Pi;
                                //Hemos guardado todos los fotones
                                if(metodo == 1){
                                    float dist = 0;
                                    float distMin = 0;

                                    for(const Photon* photone : nearPhotons) {
                                        dist = (position - photone->position()).abs();

                                        if(dist > distMin){
                                            distMin = dist;
                                        }
                                    }

                                    for(auto photon : nearPhotons) {
                                    //cambiar radio al mas cercano
                                    float aux = pi*pow(distMin,2);
                                    //cout << "kd: "; obj->getKd().print();
                                    //cout << "photon: "; photon->flux().print();

                                    Point aux2 = ((obj->getKd()/pi)*(photon->flux())/aux);
                                    //cout << "aux2: "; aux2.print();
                                    luz = luz + aux2;
                                    //Point auxiliar = photon->flux();
                                    //luz = luz + auxiliar;
                                    }

                                } else if(metodo == 2){
                                    for (int indexLuces = 0; indexLuces < nl; indexLuces++){
                                        Ray ray = Ray(position, (luces[indexLuces]->getC() - position).normal());
                                        //objeto* obj = intersecta(nf,objetos,ray,dist);

                                        //Rayos de sombra
                                        bool sombra = false;
                                        bool error;
                                        //Punto donde intersecta el rayo
                                        Direction interseccionLuzDir = (luces[indexLuces]->getC() - position);
                                        float interseccionLuzDist = interseccionLuzDir.abs();
                                        Ray interseccionLuzRay = Ray(position,interseccionLuzDir.normal());
                                        for(int index = 0; index < nf; index++ )
                                        {
                                            distancia = objetos[index]->intersection(interseccionLuzRay,error);
                                            if(!error && (distancia < interseccionLuzDist)){
                                                //cout <<"distancia: " << distancia << endl;
                                                sombra = true;
                                                break;
                                            }
                                        }

                                        if (!sombra) { // INTERSECTA
                                            //NEXT_EVENT ESTIMATION luz directa
                                            Point L = directa(luces[indexLuces]->getP(),luces[indexLuces]->getC(),position,obj->getN(r,position),obj);
                                            Point auxLuz = Point(L.x()* obj->getr(), L.y()* obj->getg(), L.z()* obj->getb());
                                            luz = luz + auxLuz;
                                        
                                        }

                                        float dist = 0;
                                        float distMin = 0;

                                        for(const Photon* photone : nearPhotons) {
                                            dist = (position - photone->position()).abs();

                                            if(dist > distMin){
                                                distMin = dist;
                                            }
                                        }

                                        //luz indirecta
                                        for(auto photon : nearPhotons) {
                                            float aux = pi*pow(distMin,2);
                                            //cout << "kd: "; obj->getKd().print();
                                            //cout << "photon: "; photon->flux().print();

                                            Point aux2 = ((obj->getKd()/pi)*(photon->flux())/aux);
                                            //cout << "aux2: "; aux2.print();
                                            luz = luz + aux2;
                                            // Point auxiliar = photon->flux();
                                            // luz = luz + auxiliar;
                                        }
                                        
                                    } 
                                    
                                } else{
                                    cout << "Metodo tiene que ser 1 ó 2" << endl;
                                }
                            }
                        }
                        
                        file << luz.x()/nRayos << " " << luz.y()/nRayos  << " "  << luz.z()/nRayos  << "     ";
                    }
                    file << endl;
                }
                file.close();
            } else {
                cerr << "No se ha podido abrir el fichero." << endl;
            }
        }

};


#endif
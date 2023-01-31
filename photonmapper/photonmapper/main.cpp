#include <iostream>

// modulos para crear la Cornell Box
#include "point.h"
#include "direction.h"
#include "plano.h"
#include "sphere.h"
#include "camara.h"
#include "ray.h"
#include "area_light.h"

// modulos para tratar las distintas imagenes
#include "img.cpp"
#include <memory>


const int calidad = 16;

int main() {
    //Escena Cornell Box.
    srand((unsigned) time(0));  
    //Planos
    Direction leftplane = Direction(1,0,0);
    Direction rightplane = Direction(-1,0,0);
    Direction floorplane = Direction(0,1,0);
    Direction ceilingplane = Direction(0,-1,0);
    Direction backplane = Direction(0,0,-1);

    Plano izq = Plano(leftplane,1, Point(1,0,0), Point(0,0,0), Point(0,0,0), 0);
    Plano dch = Plano(rightplane,1, Point(0,1,0), Point(0,0,0), Point(0,0,0), 0);
    Plano flo = Plano(floorplane,1, Point(1,1,1), Point(0,0,0), Point(0,0,0), 0);
    Plano cei = Plano(ceilingplane,1, Point(1,1,1), Point(0,0,0), Point(0,0,0), 0);
    Plano bck = Plano(backplane,1, Point(0.15,0,0), Point(0.8,0.8,0.8), Point(0,0,0), 0);

    Point OrigenSphereL = Point(0.15,0.15,0.15);
    //Mix of blue diffuse and specular
    //Sphere leftSphere = Sphere(OrigenSphereL, 0.3, Point(0.3,0.3,0), Point(0.5,0.5,0), Point(0,0,0), 0);
    Sphere leftSphere = Sphere(OrigenSphereL, 0.3, Point(0 ,0,0), Point(0,0,0), Point(1,1,1), 1.5);

    Point OrigenSphereR = Point(0,-0.7,0);
    //Mix of specular and refraction
    //Sphere rightphere = Sphere(OrigenSphereR, 0.3, Point(0,0,0), Point(0,0,0), Point(0.8,0.8,0.8), 1.5);
    Sphere rightphere = Sphere(OrigenSphereR, 0.3, Point(1,1,1), Point(0,0,0), Point(0,0,0), 1.5);
    //Light 
    Light light = Light(Point(-0.4,0.25,0), Point(1,1,1));
    Light light2 = Light(Point(0.4,-0.25,0), Point(1,1,1));
    //AreaLight light = AreaLight(cei,Point(1,1,1));
    objeto* objetos[3] = {&rightphere, &flo, &bck };
    //Conjunto de luces
    Light* luces[2] = {&light, &light2};

    // Plano izq = Plano(leftplane,1, Point(1,0,0), Point(0,0,0), Point(0,0,0), 1.5);
    // Plano dch = Plano(rightplane,1, Point(0,1,0), Point(0,0,0), Point(0,0,0), 1.5);
    // Plano flo = Plano(floorplane,1, Point(0,0,0), Point(0,0,0), Point(0,0,0), 1.5);
    // Plano cei = Plano(ceilingplane,1, Point(1,1,1), Point(0,0,0), Point(0,0,0), 1.5);
    // Plano bck = Plano(backplane,1, Point(0,0,0), Point(1,1,1), Point(0,0,0), 1.5);

    // Point OrigenSphereL = Point(-0.9,-0.1,-0.25);
    // Sphere leftSphere = Sphere(OrigenSphereL, 0.5, Point(0,0,0), Point(1,1,1), Point(0,0,0), 1.5);

    // Point OrigenSphereR = Point(0,-0.6,-0.3);
    // Sphere rightphere = Sphere(OrigenSphereR, 0.4, Point(1,0,0), Point(0,0,0), Point(0,0,0), 1.5);

    // Point OrigenSphereR2 = Point(0.35,-0,-0.3);
    // Sphere rightphere2 = Sphere(OrigenSphereR2, 0.2, Point(0,0,0), Point(0,0,0), Point(1,1,1), 1.5);

    // Point OrigenSphereR3 = Point(-0.3,0.4,-0.30);
    // Sphere rightphere3 = Sphere(OrigenSphereR3, 0.15, Point(0.3,1,0), Point(0,0,0), Point(0,0,0), 1.5);

    // Light light = Light(Point(0,0.4,0), Point(0.5,0.5,0.5));
    // //Light light2 = Light(Point(0.4,0.25,0), Point(1,1,1));
    // objeto* objetos[7] = {&rightphere, &leftSphere, &rightphere2, &rightphere3, &flo, &cei, &bck};
    // Light* luces[1] = {&light};

    //Camara
    Point OriginCam = Point(0,0,-3.5);
    Direction leftCam = Direction(-1,0,0);
    Direction upCam = Direction(0,1,0);
    Direction forwardCam = Direction(0,0,3);
    float width = 256;
    float height = 256;
    Camara cam = Camara(OriginCam, leftCam, upCam, forwardCam, width, height);

    //Obtener imagen
    //Metodo 2 next event estimation
    int metodo = 2;
    clock_t t;
    cout << "Photon mapping" << endl;
    t = clock();
    cam.pixelesPhoton(100000,(long)50,1,objetos, luces, size(objetos), size(luces), "resultado", metodo, calidad);
    t = clock() - t;
    printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    
    //cam.photon_mapping(7,objetos,1,luces,100);
    cout << "Tratando img" << endl;
    shared_ptr<image> P1(new image("resultado.ppm"));

    P1->gamma(0.2);
    P1->exportLDR("resultado_ecualizado.ppm");


    // g++ main.cpp -o main -std=c++17
    return 0;
}


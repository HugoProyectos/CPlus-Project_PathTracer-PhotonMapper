#ifndef POINT_H 
#define POINT_H

#include <string>
#include <iostream>
using namespace std;

//Definición de la clase persona -> archivo "Persona.h"
//siempre es buena idea usar macros del preprocesador para evitar compilar varias veces el mismo archivo


#include "direction.h"

class Point {
private:
    float c[4];
    Direction dir = Direction(0,0,0);

public:
    Point(float x, float y, float z){
        c[0] = x; c[1] = y; c[2] = z; c[3] = 1;
    }

    //Devuelve el valor x del pto.
    float x(){return c[0];}

    //Devuelve el valor y del pto.
    float y(){return c[1];}
    
    //Devuelve el valor z del pto.
    float z(){return c[2];}

    float h(){return c[3];}

    float n(int num)
    {
        switch (num)
        {
        case 0:
            return c[0];
            break;
        case 1:
            return c[1];
            break;
        case 2:
            return c[2];
            break;
        case 3:
            return c[3];
            break;
        default:
            break;
        }
    }

    void asignar(int num, float valor)
    {
        switch (num)
        {
        case 0:
            c[0] = valor;
            break;
        case 1:
            c[1] = valor;
            break;
        case 2:
            c[2] = valor;
            break;
        case 3:
            c[3] = valor;
            break;
        default:
            break;
        }
    }
    
    //Imprime por pantalla los valores del punto
    void print(){cout << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << endl;}

    float maximo(){
        float maximo = 0;
        for(int i = 0; i < 3; i++){
            if(maximo < c[i]){
                maximo = c[i];
            }
        }
        return maximo;
    }

    float suma(){
        return c[0] + c[1] + c[2];
    }

    //Operador de array
    float operator[](int i) const
    {
        return c[i];
    }

    //sumar direccion
    Point operator+(Point& a) const
    {
        return Point(c[0]+a.x(),c[1]+a.y(), c[2]+a.z());
    }

    Point operator+(Direction& a) const
    {
        return Point(c[0]+a.x(),c[1]+a.y(),c[2]+a.z()); 
    }
    
    Point operator-(Direction& a) const
    {
        return Point(c[0]-a.x(),c[1]-a.y(),c[2]-a.z());
    }

    Point operator/(float& a) const
    {
        return Point(c[0]/a, c[1]/a, c[2]/a);
    }

    Point operator*(float& a) const
    {
        return Point(c[0]*a, c[1]*a, c[2]*a);
    }

    Point operator*(Point a) 
    {
        return Point(c[0]*a.x(),c[1]*a.y(),c[2]*a.z()); 
    }

    //Resta de puntos = direccion
    Direction operator-(const Point& a) const
    {
        return Direction(c[0]-a.c[0],c[1]-a.c[1], c[2]-a.c[2]);
    }

    //Operador para comparar dos clases Point
    bool operator==(Point& a) const
    {
        return c[0]==a.c[0] && c[1]==a.c[1] && c[2]==a.c[2];
    }

};

#endif
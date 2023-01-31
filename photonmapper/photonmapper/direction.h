#ifndef DIRECTION_H 
#define DIRECTION_H

#include <string>
#include <iostream>
using namespace std;

#include <cmath>

//Definición de la clase persona -> archivo "Persona.h"
//siempre es buena idea usar macros del preprocesador para evitar compilar varias veces el mismo archivo

#include "point.h"

class Direction {
    
private:
    float c[4];

public:
    Direction(float x, float y, float z){
        c[0] = x; c[1] = y; c[2] = z; c[3] = 0;
    }

    //Devuelve el valor x de la direccion.
    float x(){return c[0];}
    
    //Devuelve el valor y de la direccion.
    float y(){return c[1];}
    
    //Devuelve el valor z de la direccion.
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

    //Imprime por pantalla los valores de la direccion
    void print(){cout << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << endl;};

    //Suma de direcciones
    Direction operator+(Direction a) 
    {
        return Direction(c[0]+a.x(),c[1]+a.y(),c[2]+a.z()); 
    }
    
    //Resta de direcciones
    Direction operator-(Direction a) 
    {
        return Direction(c[0]-a.x(),c[1]-a.y(),c[2]-a.z()); 
    }

    Direction operator-() 
    {
        return Direction(-c[0],-c[1],-c[2]); 
    }

    //Multiplicación de una dirección por un escalar.
    Direction operator*(float a) 
    {
        return Direction(c[0]*a,c[1]*a,c[2]*a); 
    }

    Direction operator*(Direction a) 
    {
        return Direction(c[0]*a.x(),c[1]*a.y(),c[2]*a.z()); 
    }

    /*Direction operator*(Point a) 
    {
        return Direction(c[0]*a.x(),c[1]*a.y(),c[2]*a.z()); 
    }*/

    //Division de direccion por un escalar.
    Direction operator/(float a) 
    {
        return Direction(c[0]/a,c[1]/a,c[2]/a); 
    }
    
    //Función que calcula el modulo del vector
    float abs()
    {
        return sqrt(pow(c[0],2)+pow(c[1],2)+pow(c[2],2));
    }
    //Funcion que normaliza el vector
    Direction normal()
    {
        return Direction(c[0]/abs(), c[1]/abs(), c[2]/abs());
    }

    //Operador para comparar dos clases Direccion
    bool operator==(Direction a){
        return c[0]==a.c[0] && c[1]==a.c[1] && c[2]==a.c[2];
    }


    
};
#endif
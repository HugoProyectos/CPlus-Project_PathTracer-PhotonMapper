#pragma once

#include <string>
#include <iostream>

using namespace std;

//Definición de la clase persona -> archivo "Persona.h"
//siempre es buena idea usar macros del preprocesador para evitar compilar varias veces el mismo archivo
#ifndef FUNCTION_H 
#define FUNCTION_H

#include "point.h"
#include "matrices.h"
//const float Pi = 3.14159265358979323846;
const double radianes_max1 = 175*3.14159265358979323846/180;
const double radianes_max2 = 185*3.14159265358979323846/180;
const double radianes_min1 = 5*3.14159265358979323846/180;
const double radianes_min2 = 355*3.14159265358979323846/180;
const double contraria = 180*3.14159265358979323846/180;

//Producto vectorial
// d x e = (dyez - dzey)i + (dzex - dxez)j + (dxey - dyex)k
Direction cross(Direction d1, Direction d2) 
{
    return Direction(d1.y()*d2.z() - d1.z()*d2.y(), d1.z()*d2.x() - d1.x()*d2.z(), d1.x()*d2.y() - d1.y()*d2.x());
}

//Producto escalar
//d · e = dxex + dyey + dzez
float dot(Direction d1, Direction d2) 
{
    return (d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z());
}

Direction change_base(Direction p, Point origen, Direction u, Direction v, Direction w) 
    {
        Matrix mat;
        for(int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                switch (j)
                {
                case 0:
                    mat.m[i][j] = u.n(i);
                    break;
                case 1:
                    mat.m[i][j] = v.n(i);
                    break;
                case 2:
                    mat.m[i][j] = w.n(i);
                    break;
                case 3:
                    mat.m[i][j] = origen.n(i);
                    break;
                default:
                    break;
                }
            }  
        }
        // Operacion
        //mat * punto = punto?
        //cout << "matrix" << endl;
        //mat.print();
        Direction res = mat * p;
        return res;
    }

Direction transformation(Direction normal) {
    //Direction u = Direction(normal.y(),normal.z(),normal.x());
    Direction u = Direction(0,-1,-1).normal();

    /*if((dot(normal, u) < radianes_min1 || dot(normal, u) > radianes_min2 ) || (dot(normal, u) > radianes_max2 && dot(normal, u) < radianes_max1 )){
        u = Direction(1,0,0);
    }*/

    if(dot(normal, u) == 0 || dot(normal, u) == contraria){
        u = Direction(1,1,0).normal();
    }
    
    return u;   
}


// Sus parámetros son:
    //      - el ángulo (en radianes)
    //      - el eje en que rotamos
    //      - Dir. que vamos a rotar
    Direction rotation(float o, Axis eje, Direction d)
    {
        Matrix mat;
        //Generamos matriz rotación
        //cout << "Eje:" << eje << endl;
        switch (eje)
        {
        case X:
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (i == j) 
                    {
                        if ( i == 0 || i == 3)    
                        {
                            mat.m[i][j] = 1;
                        } else
                        {
                            mat.m[i][j] = cos(o);
                            if(abs(mat.m[i][j]) < (0.0001)) {
                                mat.m[i][j] = 0;
                            }
                        }
                    } 
                    else if(i == 1 && j == 2){
                        mat.m[i][j] = -sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                            mat.m[i][j] = 0;
                        }
                    }
                    else if(i == 2 && j == 1){
                        mat.m[i][j] = sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                            mat.m[i][j] = 0;
                        }
                    }
                }
            }
            break;
        case Y:
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (i == j) 
                    {
                        if ( i == 1 || i == 3)    
                        {
                            mat.m[i][j] = 1;
                        } else
                        {
                            mat.m[i][j] = cos(o);
                            if(abs(mat.m[i][j]) < (0.0001)) {
                                mat.m[i][j] = 0;
                            }
                        }
                    } 
                    else if(i == 0 && j == 2){
                        mat.m[i][j] = sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                            mat.m[i][j] = 0;
                        }
                    }
                    else if(i == 2 && j == 0){
                        mat.m[i][j] = -sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                            mat.m[i][j] = 0;
                        }
                    }
                }
            }
            break;
        case Z:
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (i == j) 
                    {
                        if ( i == 0 || i == 1)    
                        {
                            mat.m[i][j] = cos(o);
                            if(abs(mat.m[i][j]) < (0.0001)) {
                                mat.m[i][j] = 0;
                            }
                        } else
                        {
                            mat.m[i][j] = 1;
                        }
                    } 
                    else if(i == 0 && j == 1){
                        mat.m[i][j] = -sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                                mat.m[i][j] = 0;
                        }
                    }
                    else if(i == 1 && j == 0){
                        mat.m[i][j] = sin(o);
                        if(abs(mat.m[i][j]) < (0.0001)) {
                                mat.m[i][j] = 0;
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
        

        // Operacion
        //mat * punto = punto?
        //d.print();
        //cout << endl;
        //mat.print();
        //cout << endl;
        //Direction res = mat * d;
        //return res;
    }


#endif

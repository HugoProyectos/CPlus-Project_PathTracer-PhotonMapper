#pragma once

#include <string>
#include <iostream>
#include "point.h"
#include <cmath>
using namespace std;

#ifndef MATRICES_H 
#define MATRICES_H

const int SIZE = 4;
enum Axis {X, Y, Z};

class Matrix 
{ 
    public: //ESTO no deberia ser public 
    float m[SIZE][SIZE]; //float igual no da 0, margen de error

    public:
    // Contructor para la clase Matrix sinn parametros
    Matrix(){
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++){
                m[i][j] = 0;
            } 
        }
    }

    Matrix(float matrix[SIZE][SIZE]){
        for (int i = 0; i < SIZE; i++) for (int j = 0; j < SIZE; j++) m[i][j] = matrix[i][j];
    }

    // Contructor para la clase Matrix dada una matriz
    Point traslation(Point p, Point ejes)
    {
        Matrix mat;
        //Generamos matriz traslación
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (i == j) 
                {
                    mat.m[i][j] = 1;
                } else if (j == 3)
                {
                    switch (i)
                    {
                    case 0:
                        mat.m[i][j] = ejes.x();
                        break;
                    case 1:
                        mat.m[i][j] = ejes.y();
                        break;
                    case 2:
                        mat.m[i][j] = ejes.z();
                        break;   
                    default:
                        break;
                    }
                    
                }
            }
        }

        // Operacion
        //mat.print();
        //cout << endl;
        //p.print();
        //mat * punto = punto?
        Point res = mat * p;
        return res;
    }

    Direction scale(Direction d, Direction ejes)
    {
        Matrix mat;
        //Generamos matriz escala
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (i == j) 
                {
                    switch (i)
                    {
                    case 0:
                        mat.m[i][j] = ejes.x();
                        break;
                    case 1:
                        mat.m[i][j] = ejes.y();
                        break;
                    case 2:
                        mat.m[i][j] = ejes.z();
                        break;  
                    case 3:
                        mat.m[i][j] = 1;
                        break;  
                    default:
                        break;
                    }
                } 
            }
        }

        // Operacion
        //mat * punto = punto?
        Direction res = mat * d;
        return res;
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
        Direction res = mat * d;
        return res;
    }

    
    
    /*Point operator*(Point p) {
        Point sol = Point(0,0,0);
        
        for(int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                sol.asignar(i, sol.n(i) + p.n(i) * m[i][j]);
            }  
        }
        return sol;
    }

    Direction operator*(Direction p) {
        Direction sol = Direction(0,0,0);
        
        for(int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                sol.asignar(i, sol.n(i) + p.n(i) * m[i][j]);
            }  
        }
        return sol;
    }

    Direction operator*(Direction p) {
        Direction sol = Direction(0,0,0);
        for (int row = 0; row < 4; row++) {
            //for (int col = 0; col < 4; col++) {
                // Multiply the row of A by the column of B to get the row, column of product.
                for (int inner = 0; inner < 4; inner++) {
                    sol.c[row] += m[row][inner] * p.c[inner];
                }
            //}
            
        }
        return sol;
    }*/

    float determinant(const int b){
        int c;
        float determinante = 0, aux = 0;
        float menor[4][4];
        if(b == 2){
            return ((m[0][0] * m[1][1]) - (m[1][0] * m[0][1]));
        } 
        else{
            for(int j=0; j<b; j++){
            for(int k=1; k<b; k++){
                c = 0;
                for(int l=0; l<b; l++){
                    if(l!=j){
                        menor[k-1][c] = m[k][l];
                        c++;
                    }
                }
            }

            Matrix m2 = Matrix(menor);
            aux = pow(-1, 2+j)*m[0][j]*m2.determinant(b-1);
            determinante += aux;
        }
        return determinante;
        }
        
    }

    Matrix adjunta(){
        float adj[4][4];
        int elevar = 1;
        for (int j = 0; j < SIZE; j++ ){
            for (int i = 0; i < SIZE; i++ ){
                
                //Calcular matriz 3x3
                float aux[4][4];
                int contador = 0;
                for (int l=0; l < SIZE; l++){
                    for (int n=0; n < SIZE; n++){  
                        if(l == i || j == n){
                            continue;
                        } 
                        else{
                            aux[contador/3][contador%3] = m[l][n];
                            contador++;
                        }
                        
                    }
                }

                //Tenemos la matriz 3x3
                Matrix det = Matrix(aux);
                elevar = -1;
                for(int k = 0; k <= i+j+2; k++ ){
                    elevar = elevar * (-1);
                }
                
                adj[i][j] =  elevar * det.determinant(3);
            }
        }


        return adj;
    }

    Matrix traspuesta(){
        float aux[4][4];
        for (int j = 0; j < SIZE; j++ ){
            for (int i = 0; i < SIZE; i++ ){
                aux[j][i] = m[i][j];
            }
        }
        return aux;
    }

    Matrix invertir() {
        Matrix inv = Matrix(m);
        inv = inv.adjunta().traspuesta()/inv.determinant(4);
        return inv;
    }


    Point operator*(Point p) {
        Point sol = Point(0,0,0);
        for (int row = 0; row < 4; row++) {
            sol.asignar(row, (m[row][0]*p.x()) + (m[row][1]*p.y()) + (m[row][2]*p.z()) +(m[row][3]*p.h()));
        }
        return sol;
    }

    Direction operator*(Direction p) {
        Direction sol = Direction(0,0,0);
        for (int row = 0; row < 4; row++) {
            sol.asignar(row, (m[row][0]*p.x()) + (m[row][1]*p.y()) + (m[row][2]*p.z()) +(m[row][3]*p.h()));
        }
        return sol;
    }



    void print() 
    {
        for(int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                cout << m[i][j] << " " ;
            }  
            cout << endl;
        }
    }

    Matrix operator/(const float& a) const {
        Matrix sol;
        for(int i=0; i < SIZE; i++){
            for(int j=0; j < SIZE; j++){
                sol.m[i][j] = m[i][j] / a;
            }
        }

        return sol;
    }

    Matrix operator+(const Matrix& a) const {
        Matrix sol;
        for(int i=0; i < SIZE; i++){
            for(int j=0; j < SIZE; j++){
                sol.m[i][j] = m[i][j] + a.m[i][j];
            }
        }

        return sol;
    }

    Matrix operator-(const Matrix& a) const {
        Matrix sol;
        for(int i=0; i < SIZE; i++){
            for(int j=0; j < SIZE; j++){
                sol.m[i][j] = m[i][j] - a.m[i][j];
            }
        }

        return sol;
    }

};

#endif
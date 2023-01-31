/*
 *  Fichero de Tone Mapping.
*/
#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>  
#include <iomanip> 
#include <cmath>  

using namespace std;

#ifndef Imagen_h
#define Imagen_h

const int a = 4000;

struct RGB {
    float rgb[3];
};

class image {
    private:
        RGB matrix[a][a];
        int height;
        int width;
        float max;
        int  c;

        string p;
        string maxString;
        string name;

        
        
    public:

        image(string file){
            ifstream f(file);

            getline(f, p);
            getline(f, maxString);
            getline(f, name);
            f >> width;
            f >> height;
            f >> c;

            // string::#MAX=xxxxxx => float::xxxxxx
            max = stof(maxString.erase(0, 5));
            int i = 0;
            int j = 0;
            while (!f.eof()) {
            RGB aux;
            f >> aux.rgb[0];
            f >> aux.rgb[1];
            f >> aux.rgb[2];
            aux.rgb[0] *= max/c;
            aux.rgb[1] *= max/c;
            aux.rgb[2] *= max/c;
            matrix[i][j] = (aux);
            if(j < width-1){
                j++;
            } else {
                i++;
                j = 0;
            }
            }
            f.close();
        }

        void exportLDR(string file){
            ofstream f(file);
            f << p << endl;
            f << "#MAX=1" << endl;
            f << name << endl;
            f << width << " " << height << endl;
            f << 255 << endl;
            for (int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++){
                    int r = matrix[i][j].rgb[0] * 255;
                    int g = matrix[i][j].rgb[1] * 255;
                    int b = matrix[i][j].rgb[2] * 255;
                    if(r > 255) r=255;
                    if(g > 255) g=255;
                    if(b > 255) b=255;
                    f << r << " " << g << " " << b << "     ";
                }
                f << endl;
            }
            f.close();
        }

        //Discard all values greater than 255 (1 in floating point precision)
        void clamping(float v=1.0000) {
            // cout << "Algoritmo" << endl;
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        if (matrix[i][j].rgb[k] > v) {
                            matrix[i][j].rgb[k] = v;
                        }
                    }
                }
            }
        }

        float maximo() {
            float m = 0.00000;
            // cout << "max analysis" << endl;
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        if (matrix[i][j].rgb[k] > m) {
                            m = matrix[i][j].rgb[k];
                            // cout << "Nuevo max: " << setprecision(8) << matrix[i][j].rgb[k] << endl;
                        }
                    }
                }
            }
            return m;
        }

        //Linear transformation of values from minimum to the maximum(normalization)
        void equalization() {
            float x = this->maximo();
            // cout << x << endl;
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        // cout << setprecision(8) << matrix[i][j].rgb[k] << "     " << x << "     " << matrix[i][j].rgb[k]/x << endl;
                        matrix[i][j].rgb[k] = matrix[i][j].rgb[k]/x;
                    }
                }
            }
        }

        //Combine both of the above according to a clamping parameter.
        void equalize_clamp(float v) {
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        if (matrix[i][j].rgb[k] > v) {
                            //Si es mayor que v, clamping.
                            matrix[i][j].rgb[k] = 1.0000000;
                        } else {
                            matrix[i][j].rgb[k] = matrix[i][j].rgb[k]/v;
                        }
                    }
                }
            }
        }

        //Apply a gamma curve
        void gamma(float gam) {
            this->equalization();
            cout << "img ecualizada" << endl;
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        matrix[i][j].rgb[k] = pow(matrix[i][j].rgb[k], gam);
                    }
                }
            }
        }

        //Apply a gamma curve after clamping the values
        void clamp_gamma(float gam, float v) {
            this->equalization();
            cout << "img ecualizada" << endl;
            // Comprobamos el valor s para cada pixel y RGB.
            for(int i = 0; i < height ; i++) {   //height
                //Recorremos filas de pixeles.
                for (int j = 0; j < width; j++) { //width
                    //Recorremos columnas de píxeles
                    for(int k = 0; k < 3; k++) {
                        //Recorremos RGB
                        if (matrix[i][j].rgb[k] > v) {
                            //Si es mayor que v, clamping.
                            matrix[i][j].rgb[k] = 1.0000000;
                        } else {
                            matrix[i][j].rgb[k] = pow(matrix[i][j].rgb[k], gam);
                        }
                    }
                }
            }
        }

        //No se, al hacerlo asi me da distinto 
        void clamp_gamma2(float gam, float v) {
            clamping(v);
            gamma(gam);
        }

        void help(string ppm) {
            ofstream fe (ppm);
            string cadena;
            float s = 0;
            
            if (fe.is_open()){
                fe << "P3" << endl; //P3

                fe << "#MAX=" << max << endl; // MAX ajuste

                fe << "# " << ppm << endl; //Nombre archivo

                fe << width << " " << height << endl; // Width & Height
                
                fe << setprecision(0) << fixed << c << endl;

                // Escribimos el valor s para cada pixel y RGB.
                for(int i = 0; i < height ; i++) {   //height
                    //Recorremos filas de pixeles.
                    for (int j = 0; j < width; j++) { //width
                        //Recorremos columnas de píxeles
                        for(int k = 0; k < 3; k++) {
                            //Recorremos RGB
                            fe << setprecision(8) << matrix[i][j].rgb[k] << " ";
                        }
                        fe << "    ";
                    }
                    fe << endl;
                }
                fe.close();
            } else {
                cerr << "No se ha podido abrir el archivo." << endl;
            }
        }

        void store(string ppm) {
            cout << "A escribir." << endl;
            ofstream fe (ppm);
            string cadena;
            float s = 0;
            
            if (fe.is_open()){
                fe << "P3" << endl; //P3

                fe << "#MAX=" << max << endl; // MAX ajuste

                fe << "# " << ppm << endl; //Nombre archivo

                fe << width << " " << height << endl; // Width & Height
                
                fe << setprecision(0) << fixed << c << endl;

                // Escribimos el valor s para cada pixel y RGB.
                for(int i = 0; i < height ; i++) {   //height
                    //Recorremos filas de pixeles.
                    for (int j = 0; j < width; j++) { //width
                        //Recorremos columnas de píxeles
                        //cout << setprecision(0) << "MAX " << max << "   WIDTH " << width << "  HEIGHT " << height << "  C " << c << endl;
                        for(int k = 0; k < 3; k++) {
                            //Recorremos RGB
                            fe << matrix[i][j].rgb[k]*c/max << " ";
                            //cout << "FILA " << i << "   COLUMNA " << j << "         [" << height << "][" << width << "]" << endl;
                        }

                        //cout << endl;
                        fe << "    ";
                    }
                    fe << endl;
                }
                cout << "Archivo almacenado." << endl;
                fe.close();
            } else {
                cerr << "No se ha podido abrir el archivo." << endl;
            }
        }

};

#endif
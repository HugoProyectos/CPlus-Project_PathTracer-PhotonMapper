Compilación y ejecución Photon mapping
Autores:
Diana Salueña Muñoz, 795874
Hugo Lázaro Zapata, 801758
--------------------------
#Copilación Photon Mapping
g++ main.cpp -o main -std=c++17
# Ejecución Photon Mapping
./main
-------------------------
Para modificar la escena cambiar únicamente los objetos.
Para cambiar tamaño de la imagen modificar variables "width" y "height" respectivamente.
Para modificar el número de rayos por pixel en la escena modificar variable "calidad".
Para modificar número de fotones, número de vecinos y radio de vecinos,
modificar atributos "int nPhotones, long numPhotonsNear y float radiusPhotons" en función pixelesPhoton().

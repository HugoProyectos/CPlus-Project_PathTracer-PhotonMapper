#include "img.cpp"
#include <iostream>
#include <memory>


using namespace std;

int main() {
    cout << "Tratando img" << endl;

    shared_ptr<image> P1(new image("resultado.ppm"));
    
    P1->gamma(0.2);
    P1->exportLDR("resultado_ecualizado.ppm");

    cout << "Img tratada." << endl;

    return 0;
}
/*
Description :   This is an example of usage of the KDTree class. It does not
                compile, but should give you the idea of how to integrate the
                KDTree in  code
*/


#include "kdtree.h"
#include "point.h"
#include <list>

using namespace std;

/* 
     Photon class implementation, which stores each 
    photon walk interaction 
*/
class Photon {
    private:
        //Posicion del photon
        Point position_ = Point(0,0,0);    // 3D point of the interaction
        //Photon incoming direction, de donde viene el photon
        Direction direction_ = Direction(0,0,0);
        //Flujo del photon -> numero de phtones en u area?
        Point flux_ = Point(0,0,0); 
        //...

    public:
        //Constructor
        Photon(Point p, Direction d, Point f){
            position_ = p;
            direction_ = d; 
            flux_ = f;
        }
        // It returns the axis i position (x, y or z)
        float position(size_t i) const { Point aux=position_; return aux.n(i); }
        Point position() const { return position_; }
        float direction(size_t i) const { Direction aux=direction_; aux.n(i); }
        Direction direction() const { return direction_;}
        Point flux() const { return flux_; }
        //...    
};

/* 
    An additional struct that allows the KD-Tree to access  photon position
*/
struct PhotonAxisPosition {
    float operator()(const Photon& p, size_t i) const{
        return p.position(i);
    }
};

/* 
    The KD-Tree ready to work in 3 dimensions, with Photon s, under a 
    brand-new name: PhotonMap 
*/
using PhotonMap = nn::KDTree<Photon,3,PhotonAxisPosition>;


/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(list<Photon> *photonList){
    std::list<Photon> photons = *photonList;        // Create a list of photons
    PhotonMap map = PhotonMap(photons, PhotonAxisPosition());
    return map;
}



//Example method to search for the nearest neighbors of the photon map

vector<const Photon *> search_nearest(PhotonMap map, Point position, long numPhotons, float radiusPhotons){
    // Position to look for the nearest photons
    Point query_position = position;    

    // Maximum number of photons to look for
    unsigned long nphotons_estimate = numPhotons;

    // Maximum distance to look for photons
    float radius_estimate = radiusPhotons;

    // nearest is the nearest photons returned by the KDTree
    auto nearest = map.nearest_neighbors(query_position,
                                         nphotons_estimate,
                                         radius_estimate);
    return nearest;
}



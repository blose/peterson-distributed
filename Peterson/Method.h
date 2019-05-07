#ifndef METHOD_H
#define METHOD_H
#include <vector>
#include <map>
#include "Propagator.h"
#include "definitions.h"

class Method: public Propagator {
public:
    int N;
    double tmp; //nujna dlya otladki
    
    
 
    std::vector<real> uRightHalf, uRight, uLeftHalf, uLeft;
    std::vector<real> populationHalf, population;
    real refraction;
  
    Method(const std::map<std::string,real> &data); 
    virtual std::string toString(); //preobrazuet vsyu vnutrennist' klassa v JSON stroku

    virtual void calculation(real t);
    virtual real A(real u, real population_, real x, real t);
    virtual real R(real u,real x,real t);
    virtual real rightPartPopulation(real u, real population_, int i);
};
#endif

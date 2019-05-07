#ifndef PETERSON_H
#define PETERSON_H
#include <fstream>
#include <string>
#include "Method.h"

class NeodimPeterson: public Method{
public:
     real gx;//=8.476e-7;
     real gy;
     real a;//=10.48;
     real alphaL;//=0.062675; //vnutrennie poteri
     real tauC;//=9.0e-12;
     int  longitudinalPump;
     real alphaP;
     
     static constexpr const real alphaLum=0.001;
     static constexpr const real nRefNeodim=1.8;//1.0;//2.19;
    
 //    real a1=2.08216;
 //    real alfa=0.664396;

    
    NeodimPeterson (const std::map<std::string,real> &data);
    virtual real rightPartPopulation(real u, real population_, int i);
    virtual real A(real u, real population_, real x, real t);
};

class AbsorberPeterson: public Method{
public:
     real g1;//=0.000042234;
     real alfa;//=0.664396;
     real a1;//=2.08216;
     real alphaL;//=0.062675;//0.08*10^(-2) //vnutrennie poteri
     real tauC;//=9.0e-12;
     
     static constexpr const real nRefSat=1.8;//1.0;//2.19;
    
    AbsorberPeterson (const std::map<std::string,real> &data);
    virtual real rightPartPopulation(real u, real population, int i);
    virtual real A(real u, real population, real x, real t);
};
#endif

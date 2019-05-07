#include "Method.h"
#include "Peterson.h"
#include <stdlib.h>
#include <math.h>
NeodimPeterson::NeodimPeterson(const std::map<std::string,real> &data): Method(data)
{	
    className = "NeodimPeterson";
    int i;
    real longitudinalPump, initialExcitation;
    READ(data,a);
    READ(data,gx);
    READ(data,gy);
    READ(data,alphaL);
    READ(data,tauC);
    READ(data,longitudinalPump);
    READ(data,alphaP);
    // alphaP=longitudinalPump*Sigma12Nak*N0;// =0 POPERECHNAYA NAKACHKA!!!!!!!!!!!!!!!!!!!!!!!!
    // file >> tmp >> initialExcitation;
    
    
    //I0 = new real [N];
    
    refraction=nRefNeodim;
    
    
    for (i=0; i<N; i++){
	population.push_back(0.557215 /*0.529045*/);//(-1);
    }  
    for (i=0; i<N-1; i++){
	populationHalf.push_back(0.557215 /*0.529045*/);//(-1);
    }
}

real NeodimPeterson::rightPartPopulation(real u, real population_, int i)
{
     return ((gx+gy*exp(-longitudinalPump*alphaP*i*dx))*(1-(1+exp(u))*population_)*alphaL/tauC); //prodolnaya nakachka
//с адиабатическим исключением
//    return (alphaL/tauC*g*(1-(1+exp(u)*population_) ) );
}

real NeodimPeterson::A(real u, real population_, real x, real t)
{
    return (((-1+a*population_)+exp(-u)*alphaLum*(population_)*(population_))*alphaL/tauC);   //(((-1+a*population_)*u+alphaLum*abs(population_))*alphaL/tauC/(c/nRefNeodim))

//с адиабатическим исключением
//    return (alphaL/tauC*(-1-a1/(1+exp(u)*alfa)+a*population_+exp(-u)*alphaLum*population_*population_));
    
}


AbsorberPeterson::AbsorberPeterson(const std::map<std::string,real> &data) : Method(data) {
    className = "AbsorberPeterson";
    int i;
    READ(data,g1);
    READ(data,alfa);
    READ(data,a1);
    READ(data,alphaL);
    READ(data,tauC);
    
    for (i=0; i<N; i++){
	population.push_back(-1.0);//(-1);
    }  
    for (i=0; i<N-1; i++){
	populationHalf.push_back(-1.0);//(-1);
    }
    
    refraction=nRefSat;
}

real AbsorberPeterson::rightPartPopulation(real u, real population, int i)
{
    return (g1*(-1-(1+alfa*exp(u))*population)*alphaL/tauC);
}

real AbsorberPeterson::A(real u, real population, real x, real t)
{
    return ((-1+a1*population)*alphaL/tauC); //  ((-1+a1*population)*u/(c/nRefSat)*alphaL/tauC)
}



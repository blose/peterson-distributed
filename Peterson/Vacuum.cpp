#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Vacuum.h"



Vacuum::Vacuum(const std::map<std::string,real> &data) : Propagator()
{
    className = "Vacuum";
    READ(data,dt);
    READ(data,Length)
    dx=dt*c;
    if (dx *10 > Length) {
	std::cerr << "Warning!!! less then 10 points in Vacuum!!!" <<  std::endl;
    }
    N=lroundl(Length/dx);
    intensitiesLeft = new real [N];
    intensitiesRight = new real [N];
    pointer =0;
    
    
    int i;
    for(i=0;i<N; i++){
	intensitiesLeft[i]=0.0;//0.8;//%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	intensitiesRight[i]=0.0;//0.8;//%%%%%%%%%%%%%%%%5
    };
    in_Left=intensitiesLeft;
    in_Right=intensitiesRight;
}


void Vacuum::calculation(real t)
{
    pointer ++;
    if (pointer == N) pointer=0;
    
    
    in_Left=&(intensitiesLeft[pointer]);//mesto, kuda my pishem novoe znachenie intensivnosti
    in_Right=&(intensitiesRight[pointer]);
    
    out_left=intensitiesLeft[pointer];
    out_right=intensitiesRight[pointer];
}

std::string Vacuum::toString() 
{
    std::stringstream s;
    s << "\"Vacuum\" : { \"outRight\":" << out_right << 
    ", \"outLeft\": " << out_left << 
    ", \"N\": " << N <<
    ", \"pointer\": "<< pointer;
    int i;
    s << ", \"intensitiesLeft\": [";
    for(i=0;i<N;i++) {
	if (i) {
	    s << ",";
	}
	s << intensitiesLeft[i];
	
    }
    s << "], \"intensitiesRight\": [";
    for(i=0;i<N;i++) {
	if (i) {
	    s << ",";
	}
	s << intensitiesRight[i];
	
    }
    s << "] } ";
    return s.str();
}

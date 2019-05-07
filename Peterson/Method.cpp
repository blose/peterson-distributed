#include <iostream>
#include <math.h>
#include "definitions.h"
#include "Method.h"
#include <sstream>

// resets negative values in array
// return value
// 0 reset was not commited
// 1 reset was commited

template<typename T>
int resetNegativeValues(std::vector<T> &arr) {
    int resetMode = 0;
    // 0 - do nothing
    // 1 - reset whole array to zero
    // 2 - reset negative values to averages
    
    int i,reset = 0;
    
    if (resetMode == 1) {
	int size = arr.size();
	for (i=0;i<size;i++) {
	    if (arr[i]<0) {
		reset = 1;
		break;
	    }
	}
	if (reset) {
	    for (i=0;i<size;i++) {
		arr[i] = 0;
		
	    }
	}
    }  
    
    if (resetMode == 2) {
	int size = arr.size();
	for (i=1;i<(size-1);i++) {
	    if (arr[i]<0) {
		arr[i] = 0.5*(arr[i+1]+arr[i-1]);
		reset = 1;
	    }
	}
    }
    return reset;
    
}

Method::Method(const std::map<std::string,real> &data) : Propagator() {
    className = "Method";
    READ(data,dt); 
    READ(data,Length);
    dx=2.0*dt*c;//speed_of_light;
    if (dx *30 > Length) {
	std::cout << "Warning!!! less then 30 points in Method !!!" <<  std::endl;
    }
    
    N=lroundl(Length/dx)+2;
    std::cout<<N<< std::endl;
    
    int i;
    for (i=0;i<N;i++) {
	uRight.push_back(-0.923013 /*0.809456*/);
	uLeft.push_back(-0.923013 /*0.809456*/);
	uRightHalf.push_back(-0.923013 /*0.809456*/);
	uLeftHalf.push_back(-0.923013 /*0.809456*/);
    }
    
    out_left=-0.923013;
    out_right=-0.923013;
    
    in_Right=&(uRight[0]);
    in_Left=&(uLeft[0]);
    
};

std::string Method::toString()
{
    std::stringstream s;
    s << "\"" << className  << "\": { \"outRight\":" << out_right << 
    ", \"outLeft\": " << out_left << 
    ", \"N\": " << N;
    int i;
    s << ", \"uLeft\": [";
    for(i=0;i<N;i++) {
	if (i) {
	    s << ",";
	}
	s << uLeft[i];
	
    }
    s << "], \"uRight\": [";
    for(i=0;i<N;i++) {
	if (i) {
	    s << ",";
	}
	s << uLeft[i];
	
    }
    s << "], \"population\": [";
    for(i=0;i<N;i++) {
	if (i) {
	    s << ",";
	}
	s << population[i];
	
    }
    s << "] } ";
    
    return s.str();
}


void Method::calculation(real t)
{
    // dt/2 layer
    
    
    int i;
    //#pragma omp parallel for
    for (i=0; i<=N-2; i++){
	populationHalf[i]=0.5*(population[i]+population[i+1]);
    };
    //    populationHalf[N-1] = 0;//populationHalf[N-2];
    
    //pryamaya volna
    //#pragma omp parallel for
    for (i=0; i<=N-2; i++){    
	real R1,R2,A1,A2;
	R1 = R(uRight[i+1],(i+1)*dx,t);
	R2 = R(uRight[i],i*dx,t);
	A1 = A(uRight[i],population[i],i*dx,t);
	A2 = A(uRight[i+1],population[i+1],(i+1)*dx,t);
	uRightHalf[i]=0.5*(uRight[i]+uRight[i+1])+0.5*dt*(  (R1-R2)/(-dx)+0.5*(A1+A2)  );
    };
    resetNegativeValues(uRightHalf);
    
    //obratnaya volna
    //#pragma omp parallel for
    for (i=0; i<=N-2; i++){    
	real R1,R2,A1,A2;
	R1 = R(uLeft[i+1],(i+1)*dx,t);
	R2 = R(uLeft[i],i*dx,t);
	A1 = A(uLeft[i],population[N-(i)-1],i*dx,t);
	A2 = A(uLeft[i+1],population[N-(i+1)-1],(i+1)*dx,t);
	uLeftHalf[i]=0.5*(uLeft[i]+uLeft[i+1])+0.5*dt*(  (R1-R2)/(-dx)+0.5*(A1+A2)  );
    };
    resetNegativeValues(uLeftHalf);
    
    
    //sloj dt
    //Euler
    // n 3 2 1 0
    // x 0 1 2 3 
    // N = 4 -> N-1 = 3    
    
    //#pragma omp parallel for
    for(i=0; i<=N-1; i++){
	population[i]=population[i]+dt*rightPartPopulation(uRight[i]+uLeft[N-i-1],population[i], i);
    };
    
//    resetNegativeValues(population);
    
    //pryamaya volna
    
    //#pragma omp parallel for
    for (i=1; i<=N-2; i++){
	real R1, R2, A1, A2;
	R1 = R(uRightHalf[i],(i+0.5)*dx,t);
	R2 = R(uRightHalf[i-1],(i-0.5)*dx,t);
	A1 = A(uRightHalf[i-1],populationHalf[i-1],(i-0.5)*dx,t);
	A2 = A(uRightHalf[i],populationHalf[i],(i+0.5)*dx,t);
	uRight[i]=uRight[i]+dt*((R1-R2)/(-dx)+0.5*(A1+A2));
    };   
    
    resetNegativeValues(uRight);
    
    //obpryamaya volna
    //#pragma omp parallel for
    for (i=1; i<=N-2; i++){     /*ОШИБКА ТУТ!!!!!!!!!!!!!!11*/
	real R1, R2, A1, A2;
	R1 = R(uLeftHalf[i],(i+0.5)*dx,t);
	R2 = R(uLeftHalf[i-1],(i-0.5)*dx,t);
	A1 = A(uLeftHalf[i-1],populationHalf[N-(i-1)-1],(i-0.5)*dx,t);
	A2 = A(uLeftHalf[i],populationHalf[N-(i)-1],(i+0.5)*dx,t);
	uLeft[i]=uLeft[i]+dt*((R1-R2)/(-dx)+0.5*(A1+A2));
    };
    
    resetNegativeValues(uLeft);
    
    out_right=uRight[N-2];
    out_left=uLeft[N-2];
    
    uRight[N-1] = uRight[N-2];
    uLeft[N-1] = uLeft[N-2];
}

real Method::A(real u, real population_, real x, real t)
{
    return 0;
}

real Method::R(real u, real x, real t)
{
    return c/refraction*u;
}

real Method::rightPartPopulation(real u, real population_, int i)
{
    return 0;
}

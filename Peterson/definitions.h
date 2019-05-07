#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <map>
#include <exception>

#define OM_JSON		0
#define OM_ASCII	1
#define OM_BINARY	2

#define sqr(o) ((o)*(o))



typedef double real;


#define READ(MAP,VAR) if (MAP.find(#VAR)!=MAP.end()) VAR = (*MAP.find(#VAR)).second; else throw "ERROR: Parameter " #VAR " not found";


const real hPlank=6.63e-34;
const real c=3.0e8;
const real px=3.21e-6;
const real py=2.8e-6;
const real sx=4.39e-6;
const real sy=4.159e-6;

//	const real px=200.0e-6;//3.8e-6;
//      const real py=200.0e-6;//7.4e-6;
//      const real sx=200.0e-6;//6.5e-6;
//      const real sy=200.0e-6;//8.8e-6;


const real LambdaNak=814.0e-9;//808.0e-9;
const real LambdaGen=1080.0e-9;//1340.0e-9;

const real NuNak = c/LambdaNak;
const real NuGen = c/LambdaGen;
const real Sp=px*py;
const real Ss=sx*sy;

const real ksi=1;

const long int nskip=1;
#endif

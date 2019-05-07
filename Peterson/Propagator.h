#ifndef PROPAGATOR_H
#define PROPAGATOR_H
#include <string>
#include "definitions.h"


class Propagator {
public: 
  //polya classa
  //real left;
  std::string className;
  real Length;
  real dt, dx;
  real out_left;  // intensivnost' out volny vlevo
  real out_right; // intensivnost' out volny vpravo
  
  real r_left, r_right;
  real *in_Left, *in_Right;
  
  //metody classa
  Propagator();
  ~Propagator();// destructor
  virtual void calculation(real t);
  
  virtual std::string toString(); //preobrazuet vsyu vnutrennist' klassa v JSON stroku
};

#endif

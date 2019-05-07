#ifndef VACUUM_H
#define VACUUM_H
#include <string>
#include <map>
#include "definitions.h"
#include "Propagator.h"


class Vacuum: public Propagator {
public:
    real *intensitiesLeft, *intensitiesRight;
    int N;//kol-vo tochek v vacuume
    int pointer;
  
  Vacuum(const std::map<std::string,real> &data);
  virtual void calculation(real t);
  
  virtual std::string toString(); //preobrazuet vsyu vnutrennist' klassa v JSON stroku

};
#endif

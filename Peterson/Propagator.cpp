#include <string>
#include <sstream>
#include "Propagator.h"

Propagator::Propagator() : className("Propagator")
{
    out_right=0.0;
    out_left=0.0;
}

Propagator::~Propagator() {}

void Propagator::calculation(real t) {}

std::string Propagator::toString()
{
    std::stringstream s;
    s << "\"Propagator\" : { \"outRight\":"<<  out_right << ", \"outLeft\": " <<out_left << " }";
    return s.str();
}
    
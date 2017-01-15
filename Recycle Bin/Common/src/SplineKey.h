#ifndef SplineKey_h
#define SplineKey_h

#include "mymath/mymath.h"

struct SplineKey
{
  mm::vec3 f; //value of the funciton
  float t; //time value
  float tens, bias, cont; //tension, bias, continuity

  SplineKey() : t( 0 ), tens( 0 ), bias( 0 ), cont( 0 ) {}

  SplineKey( const mm::vec3& f0, float t0, float tens0 = 0, float bias0 = 0, float cont0 = 0 )
    : f( f0 ), t( t0 ), tens( tens0 ), bias( bias0 ), cont( cont0 ) {}
};

#endif

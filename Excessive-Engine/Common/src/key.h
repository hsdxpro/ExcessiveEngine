#ifndef spline_key_h
#define spline_key_h

#include "mymath/mymath.h"

struct key
{
  mm::vec3 f; //value of the funciton
  f32 t; //time value
  f32 tens, bias, cont; //tension, bias, continuity

  key() : t( 0 ), tens( 0 ), bias( 0 ), cont( 0 ) {}

  key( const mm::vec3& f0, f32 t0, f32 tens0 = 0, f32 bias0 = 0, f32 cont0 = 0 )
    : f( f0 ), t( t0 ), tens( tens0 ), bias( bias0 ), cont( cont0 ) {}
};

#endif

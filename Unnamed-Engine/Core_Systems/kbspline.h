#ifndef kbspline_h
#define kbspline_h

#include "key.h"
#include <vector>

class segment
{
  private:
    mm::vec3 a, b, c, d;  //polynomial
    f32 tstart, Dt; //start time, lenght
  public:
    //computing coefficients
    void init( mm::vec3& f0, mm::vec3& v0, f32 t0,
               mm::vec3& f1, mm::vec3& v1, f32 t1 )
    {
      tstart = t0;
      Dt = t1 - t0;
      a = ( v1 + v0 ) / ( Dt * Dt ) - ( f1 - f0 ) * 2 / ( Dt * Dt * Dt );
      b = ( f1 - f0 ) * 3 / ( Dt * Dt ) - ( v1 + v0 * 2 ) / Dt;
      c = v0;
      d = f0;
    }

    //interpolation
    mm::vec3 value( f32 t )
    {
      f32 T = t - tstart;
      return ( a * T * T * T + b * T * T + c * T + d );
    }
};

class KochanekBartelsSpline
{
  private:
    std::vector<key> keys; //array of keys
    std::vector<segment> segments; //array of segments
    f32 fullTime;
    mm::vec3 vstart, vend;

  public:
    void setStartVelocity( const mm::vec3& vs )
    {
      vstart = vs;
    }

    void setEndVelocity( const mm::vec3& ve )
    {
      vend = ve;
    };

    void addKey( const key& newkey );
    f32 getFullTime() const;
    void recalc();
    bool removeKey( const key& rkey );
    bool removeKeyById( i32 id );
    mm::vec3 value( f32 t ); //interpolated value

    KochanekBartelsSpline() : fullTime( 0 ), vstart( 0 ), vend( 0 ) {}
};

#endif // kbspline_h__

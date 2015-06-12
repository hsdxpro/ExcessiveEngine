#ifndef kbspline_h
#define kbspline_h

#include "SplineKey.h"
#include <vector>

class segment
{
  private:
    mm::vec3 a, b, c, d;  //polynomial
    float tstart, Dt; //start time, lenght
  public:
    //computing coefficients
    void init( mm::vec3& f0, mm::vec3& v0, float t0,
               mm::vec3& f1, mm::vec3& v1, float t1 )
    {
      tstart = t0;
      Dt = t1 - t0;
      a = ( v1 + v0 ) / ( Dt * Dt ) - ( f1 - f0 ) * 2 / ( Dt * Dt * Dt );
      b = ( f1 - f0 ) * 3 / ( Dt * Dt ) - ( v1 + v0 * 2 ) / Dt;
      c = v0;
      d = f0;
    }

    //interpolation
    mm::vec3 value( float t )
    {
      float T = t - tstart;
      return ( a * T * T * T + b * T * T + c * T + d );
    }
};

class KochanekBartelsSpline
{
  private:
    std::vector<SplineKey> keys; //array of keys
    std::vector<segment> segments; //array of segments
    float fullTime;
    mm::vec3 vstart, vend;

  public:
    void SetStartVelocity( const mm::vec3& vs )
    {
      vstart = vs;
    }

    void SetEndVelocity( const mm::vec3& ve )
    {
      vend = ve;
    };

    void AddKey( const SplineKey& newkey );
    float GetFullTime() const;
    void recalc();
    bool RemoveKey( const SplineKey& rkey );
    bool RemoveKeyById( int id );
    mm::vec3 value( float t ); //interpolated value

    KochanekBartelsSpline() : fullTime( 0 ), vstart( 0 ), vend( 0 ) {}
};

#endif // kbspline_h__

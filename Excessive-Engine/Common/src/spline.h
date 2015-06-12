#ifndef Spline_h
#define Spline_h

#include "kbspline.h"

template< class t >
class Spline
{
    t s;
  public:
    void SetStartSelocity( const mm::vec3& vstart )
    {
      s.SetStartVelocity( vstart );
    }

    void SetEndVelocity( const mm::vec3& vend )
    {
      s.SetEndVelocity( vend );
    }

    void AddKey( const SplineKey& newkey )
    {
      s.AddKey( newkey );
    }

    bool RemoveKey( const SplineKey& rkey )
    {
      return s.RemoveKey( rkey );
    }

    bool RemoveKeyById( int id )
    {
      return s.RemoveKeyById( id );
    }

    float GetFullTime() const
    {
      return s.GetFullTime();
    }

    void recalc()
    {
      s.recalc();
    }

    mm::vec3 value( float tt )
    {
      return s.value( tt );
    }
};

typedef Spline<KochanekBartelsSpline> splinetype;

#endif // spline_h__

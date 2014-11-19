#ifndef Spline_h
#define Spline_h

#include "kbspline.h"

template< class t >
class Spline
{
    t s;
  public:
    void setStartSelocity( const mm::vec3& vstart )
    {
      s.setStartVelocity( vstart );
    }

    void setEndVelocity( const mm::vec3& vend )
    {
      s.setEndVelocity( vend );
    }

    void addKey( const SplineKey& newkey )
    {
      s.addKey( newkey );
    }

    bool removeKey( const SplineKey& rkey )
    {
      return s.removeKey( rkey );
    }

    bool removeKeyById( int id )
    {
      return s.removeKeyById( id );
    }

    float getFullTime() const
    {
      return s.getFullTime();
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

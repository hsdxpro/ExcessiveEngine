#ifndef spline_h
#define spline_h

#include "basic_types.h"
#include "kbspline.h"

template< class t >
class spline
{
    t s;
  public:
    void set_start_velocity( const mm::vec3& vstart )
    {
      s.setStartVelocity( vstart );
    }

    void set_end_velocity( const mm::vec3& vend )
    {
      s.setEndVelocity( vend );
    }

    void add_key( const key& newkey )
    {
      s.addKey( newkey );
    }

    bool remove_key( const key& rkey )
    {
      return s.removeKey( rkey );
    }

    bool remove_key_by_id( i32 id )
    {
      return s.removeKeyById( id );
    }

    f32 get_full_time() const
    {
      return s.getFullTime();
    }

    void recalc()
    {
      s.recalc();
    }

    mm::vec3 value( f32 tt )
    {
      return s.value( tt );
    }
};

typedef spline<KochanekBartelsSpline> splinetype;

#endif // spline_h__

#ifndef mm_quat_func_h
#define mm_quat_func_h

#include "mm_quat_impl.h"

template<typename ty>
mymath::impl::quati<ty> operator*( const mymath::impl::quati<ty>& p, const mymath::impl::quati<ty>& q )
{
  const mymath::impl::vec3i<ty> pv = p.value.xyz;
  const ty                      ps = p.value.w;
  const mymath::impl::vec3i<ty> qv = q.value.xyz;
  const ty                      qs = q.value.w;

  return mymath::impl::quati<ty>(mymath::impl::vec4i<ty>( ps * qv + qs * pv + mymath::cross( pv, qv ),
    ps * qs - mymath::dot( pv, qv ) ));
}

template<typename ty>
mymath::impl::quati<ty> operator*( const mymath::impl::quati<ty>& p, const ty& num )
{
  return mymath::impl::quati<ty>(p.value * num);
}

template<typename ty>
mymath::impl::quati<ty> operator*( const ty& num, const mymath::impl::quati<ty>& p )
{
  return p * num;
}

template<typename ty>
mymath::impl::quati<ty> operator/( const mymath::impl::quati<ty>& p, const ty& num )
{
  return mymath::impl::quati<ty>(p.value / num);
}

namespace mymath
{
  template<typename ty>
  impl::quati<ty> conjugate( const impl::quati<ty>& q )
  {
    return mymath::impl::quati<ty>(mymath::impl::vec4i<ty>(-q.value.xyz, q.value.w));
  }

  template<typename ty>
  impl::quati<ty> inverse( const impl::quati<ty>& q )
  {
    //return conjugate(q) / dot_helper(q.value, q.value);
    const ty lensqr = dot( q.value, q.value );
    assert( lensqr != ty( 0 ) );
    return conjugate( q ) / lensqr;
  }

  template<typename ty>
  impl::quati<ty> normalize( const impl::quati<ty>& q )
  {
    return impl::quati<ty>(normalize( q.value ));
  }

  template<typename ty>
  ty norm( const impl::quati<ty>& q )
  {
    return length( q.value );
  }

  template<typename ty>
  impl::mat3i<ty> mat3_cast( const impl::quati<ty>& q )
  {
    vec3 coloumn1 = q.value.yyx * q.value.yxz * vec3( 2 ) * vec3( -1, 1, 1 );
    vec3 tmp2 = q.value.zzy * q.value.zww * vec3( 2 ) * vec3( -1, -1, 1 );
    coloumn1 = coloumn1 + tmp2 + vec3( 1, 0, 0 );

    vec3 coloumn2 = q.value.xxy * q.value.yxz * vec3( 2 ) * vec3( 1, -1, 1 );
    vec3 tmp4 = q.value.zzx * q.value.wzw * vec3( 2 ) * vec3( 1, -1, -1 );
    coloumn2 = coloumn2 + tmp4 + vec3( 0, 1, 0 );

    vec3 coloumn3 = q.value.xyx * q.value.zzx * vec3( 2 ) * vec3( 1, 1, -1 );
    vec3 tmp6 = q.value.yxy * q.value.wwy * vec3( 2 ) * vec3( -1, 1, -1 );
    coloumn3 = coloumn3 + tmp6 + vec3( 0, 0, 1 );

    return mat3(
      coloumn1.x, coloumn2.x, coloumn3.x,
      coloumn1.y, coloumn2.y, coloumn3.y,
      coloumn1.z, coloumn2.z, coloumn3.z
      );
  }

  template<typename ty>
  impl::mat4i<ty> mat4_cast( const impl::quati<ty>& q )
  {
    return impl::mat4i<ty>( mat3_cast<ty>( q ) );
  }

  ///This function does a linear interpolation
  ///
  template<typename ty>
  impl::quati<ty> mix( const impl::quati<ty>& q1, const impl::quati<ty>& q2, const ty& t )
  {
    return impl::quati<ty>(normalize( q1.value*( 1 - t ) + q2.value*t ));
  }

  template<typename ty>
  impl::quati<ty> slerp( const impl::quati<ty>& q1, const impl::quati<ty>& q2, const ty& t )
  {
    assert( all( equal( q1.value, q2.value ) ) == false && all( equal( q1.value, -q2.value ) ) == false );

    float theta = std::acos( dot( q1.value, q2.value ) );
    float sintheta = std::sin( theta );
    float wp = std::sin( ( 1 - t ) * theta ) / sintheta;
    float wq = std::sin( t * theta ) / sintheta;
    return impl::quati<ty>(wp * q1.value + wq * q2.value);
  }

  //TODO SLOW
  template<typename ty>
  impl::quati<ty> quat_cast( const impl::mat3i<ty>& m )
  {
    ty trace = m[0][0] + m[1][1] + m[2][2];

    impl::quati<ty> result;

    //If the trace of the matrix is greater than zero, then the result is
    if( trace > 0 )
    {
      /* X = ( m21 - m12 ) * S
       * Y = ( m02 - m20 ) * S
       * Z = ( m10 - m01 ) * S
       * our matrices are column-major so the indexing is reversed
       */

      ty s = 0.5 / std::sqrt( trace + 1 );
      result.value.w = 0.25 / s;
      result.value.x = ( m[1][2] - m[2][1] ) * s;
      result.value.y = ( m[2][0] - m[0][2] ) * s;
      result.value.z = ( m[0][1] - m[1][0] ) * s;
    }
    else
    {
      if( m[0][0] > m[1][1] && m[0][0] > m[2][2] )
      {
        ty s = 2 * std::sqrt( 1 + m[0][0] - m[1][1] - m[2][2] );

        result.value.w = ( m[1][2] - m[2][1] ) / s;
        result.value.x = 0.25 * s;
        result.value.y = ( m[1][0] + m[0][1] ) / s;
        result.value.z = ( m[2][0] + m[0][2] ) / s;
      }
      else if( m[1][1] > m[2][2] )
      {
        ty s = 2 * std::sqrt( 1 + m[1][1] - m[0][0] - m[2][2] );

        result.value.w = ( m[2][0] - m[0][2] ) / s;
        result.value.x = ( m[1][0] + m[0][1] ) / s;
        result.value.y = 0.25 * s;
        result.value.z = ( m[2][1] + m[1][2] ) / s;
      }
      else
      {
        ty s = 2 * std::sqrt( 1 + m[2][2] - m[0][0] - m[1][1] );

        result.value.w = ( m[0][1] - m[1][0] ) / s;
        result.value.x = ( m[2][0] + m[0][2] ) / s;
        result.value.y = ( m[2][1] + m[1][2] ) / s;
        result.value.z = 0.25 * s;
      }
    }

    return result;
  }

  template<typename ty>
  impl::quati<ty> quat_cast( const impl::mat4i<ty>& m )
  {
    return quat_cast( impl::mat3i < ty >( m ) );
  }

  template<typename ty>
  impl::vec3i<ty> rotate_vector( const impl::quati<ty>& q, const impl::vec3i<ty>& v )
  {
    impl::quati<ty> v_quat( v );

    v_quat = q * v_quat * inverse( q );
    return v_quat.value.xyz;
  }

  template<typename ty>
  impl::quati<ty> get_rotation( const impl::vec3i<ty>& start_dir, const impl::vec3i<ty>& end_dir )
  {
    impl::quati<ty> result;

    const ty angle = std::acos( dot( start_dir, end_dir ) );
    const impl::vec3i<ty> axis = mymath::cross( start_dir, end_dir );
    if( mymath::all( mymath::equal( axis, vec3( 0, 0, 0 ) ) ) )
    {
      assert( angle < mymath::epsilon );
      result = impl::quati<ty>( angle, vec3( 1, 0, 0 ) );
    }
    else
    {
      result = impl::quati<ty>( angle, axis );
    }

    return result;
  }

}

#endif

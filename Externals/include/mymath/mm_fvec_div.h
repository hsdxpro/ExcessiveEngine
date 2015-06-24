namespace mymath
{
  namespace impl
  {
    vec2i<float>::vec2i( const vec2i<int>& v ){ x = v.x; y = v.y; }
    vec2i<float>::vec2i( const vec2i<unsigned>& v ){ x = v.x; y = v.y; }
    vec3i<float>::vec3i( const vec3i<int>& v ){ x = v.x; y = v.y; z = v.z; }
    vec3i<float>::vec3i( const vec3i<unsigned>& v ){ x = v.x; y = v.y; z = v.z; }
    vec4i<float>::vec4i( const vec4i<int>& v ){ x = v.x; y = v.y; z = v.z; w = v.w; }
    vec4i<float>::vec4i( const vec4i<unsigned>& v ){ x = v.x; y = v.y; z = v.z; w = v.w; }
  }

  MYMATH_INLINE impl::vec2i<bool> isnan( const impl::vec2i<float>& vec )
  { return impl::vec2i<bool>( isnan( vec.x ), isnan( vec.y ) ); }
  MYMATH_INLINE impl::vec3i<bool> isnan( const impl::vec3i<float>& vec )
  {  return impl::vec3i<bool>( isnan( vec.x ), isnan( vec.y ), isnan( vec.z ) ); }
  MYMATH_INLINE impl::vec4i<bool> isnan( const impl::vec4i<float>& vec )
  { return impl::vec4i<bool>( isnan( vec.x ), isnan( vec.y ), isnan( vec.z ), isnan( vec.w ) ); }

  //isinf
  MYMATH_INLINE impl::vec2i<bool> isinf( const impl::vec2i<float>& vec )
  { return impl::vec2i<bool>( isinf( vec.x ), isinf( vec.y ) ); }
  MYMATH_INLINE impl::vec3i<bool> isinf( const impl::vec3i<float>& vec )
  { return impl::vec3i<bool>( isinf( vec.x ), isinf( vec.y ), isinf( vec.z ) ); }
  MYMATH_INLINE impl::vec4i<bool> isinf( const impl::vec4i<float>& vec )
  { return impl::vec4i<bool>( isinf( vec.x ), isinf( vec.y ), isinf( vec.z ), isinf( vec.w ) ); }

  //less
  MYMATH_INLINE impl::vec2i<bool> lessThan( const mm::impl::vec2i<float>& a, const mm::impl::vec2i<float>& b )
  {
    mm::impl::vec2i<float> v = _mm_cmplt_ps( a.d, b.d );
    return impl::vec2i<bool>( v.x, v.y );
  }
  MYMATH_INLINE impl::vec3i<bool> lessThan( const mm::impl::vec3i<float>& a, const mm::impl::vec3i<float>& b )
  {
    mm::impl::vec3i<float> v = _mm_cmplt_ps( a.d, b.d );
    return impl::vec3i<bool>( v.x, v.y, v.z );
  }
  MYMATH_INLINE impl::vec4i<bool> lessThan( const mm::impl::vec4i<float>& a, const mm::impl::vec4i<float>& b )
  {
    mm::impl::vec4i<float> v = _mm_cmplt_ps( a.d, b.d );
    return impl::vec4i<bool>( v.x, v.y, v.z, v.w );
  }

  //greater
  MYMATH_INLINE impl::vec2i<bool> greaterThan( const mm::impl::vec2i<float>& a, const mm::impl::vec2i<float>& b )
  {
    mm::impl::vec2i<float> v = _mm_cmpgt_ps( a.d, b.d );
    return impl::vec2i<bool>( v.x, v.y );
  }
  MYMATH_INLINE impl::vec3i<bool> greaterThan( const mm::impl::vec3i<float>& a, const mm::impl::vec3i<float>& b )
  {
    mm::impl::vec3i<float> v = _mm_cmpgt_ps( a.d, b.d );
    return impl::vec3i<bool>( v.x, v.y, v.z );
  }
  MYMATH_INLINE impl::vec4i<bool> greaterThan( const mm::impl::vec4i<float>& a, const mm::impl::vec4i<float>& b )
  {
    mm::impl::vec4i<float> v = _mm_cmpgt_ps( a.d, b.d );
    return impl::vec4i<bool>( v.x, v.y, v.z, v.w );
  }

  //less or equal
  MYMATH_INLINE impl::vec2i<bool> lessThanEqual( const mm::impl::vec2i<float>& a, const mm::impl::vec2i<float>& b )
  {
    mm::impl::vec2i<float> v = _mm_cmple_ps( a.d, b.d );
    return impl::vec2i<bool>( v.x, v.y );
  }
  MYMATH_INLINE impl::vec3i<bool> lessThanEqual( const mm::impl::vec3i<float>& a, const mm::impl::vec3i<float>& b )
  {
    mm::impl::vec3i<float> v = _mm_cmple_ps( a.d, b.d );
    return impl::vec3i<bool>( v.x, v.y, v.z );
  }
  MYMATH_INLINE impl::vec4i<bool> lessThanEqual( const mm::impl::vec4i<float>& a, const mm::impl::vec4i<float>& b )
  {
    mm::impl::vec4i<float> v = _mm_cmple_ps( a.d, b.d );
    return impl::vec4i<bool>( v.x, v.y, v.z, v.w );
  }

  //greater or equal
  MYMATH_INLINE impl::vec2i<bool> greaterThanEqual( const mm::impl::vec2i<float>& a, const mm::impl::vec2i<float>& b )
  {
    mm::impl::vec2i<float> v = _mm_cmpge_ps( a.d, b.d );
    return impl::vec2i<bool>( v.x, v.y );
  }
  MYMATH_INLINE impl::vec3i<bool> greaterThanEqual( const mm::impl::vec3i<float>& a, const mm::impl::vec3i<float>& b )
  {
    mm::impl::vec3i<float> v = _mm_cmpge_ps( a.d, b.d );
    return impl::vec3i<bool>( v.x, v.y, v.z );
  }
  MYMATH_INLINE impl::vec4i<bool> greaterThanEqual( const mm::impl::vec4i<float>& a, const mm::impl::vec4i<float>& b )
  {
    mm::impl::vec4i<float> v = _mm_cmpge_ps( a.d, b.d );
    return impl::vec4i<bool>( v.x, v.y, v.z, v.w );
  }

  MYMATH_INLINE impl::vec2i<bool> equal( const impl::vec2i<float>& a, const impl::vec2i<float>& b )
  {
    return lessThan( mm::abs( a - b ), impl::vec2i<float>( epsilon ) );
  }
  MYMATH_INLINE impl::vec3i<bool> equal( const impl::vec3i<float>& a, const impl::vec3i<float>& b )
  {
    return lessThan( mm::abs( a - b ), impl::vec3i<float>( epsilon ) );
  }
  MYMATH_INLINE impl::vec4i<bool> equal( const impl::vec4i<float>& a, const impl::vec4i<float>& b )
  {
    return lessThan( mm::abs( a - b ), impl::vec4i<float>( epsilon ) );
  }

  MYMATH_INLINE impl::vec2i<bool> notEqual( const impl::vec2i<float>& a, const impl::vec2i<float>& b )
  {
    return not( equal( a, b ) );
  }
  MYMATH_INLINE impl::vec3i<bool> notEqual( const impl::vec3i<float>& a, const impl::vec3i<float>& b )
  {
    return not( equal( a, b ) );
  }
  MYMATH_INLINE impl::vec4i<bool> notEqual( const impl::vec4i<float>& a, const impl::vec4i<float>& b )
  {
    return not( equal( a, b ) );
  }

  namespace impl
  {
    template<int ta, int tb, int tc, int td>
    const vec2i<float>& vec2i<float>::swizzle<ta, tb, tc, td>::operator/=( const vec2i<float>& vec )
    {
      assert( all( notEqual( vec, vec2i<float>( 0 ) ) ) );
      vec2i<float>* tmp = (vec2i<float>*)this;
      tmp->d = _mm_div_ps( v, _mm_shuffle_ps( vec.d, vec.d, MYMATH_SHUFFLE( ta, tb, 0, 0 ) ) );
      return *( vec2i<float>* )this;
    }

#ifndef MYMATH_FAST_COMPILE
    template<int at, int bt, int ct, int dt>
    const vec3i<float>& vec3i<float>::swizzle<at, bt, ct, dt>::operator/=( const vec3i<float>& vec )
    {
      assert( all( notEqual( vec, vec3i<float>( 0 ) ) ) );
      vec3i<float>* tmp = (vec3i<float>*)this;
      tmp->d = _mm_div_ps( v, _mm_shuffle_ps( vec.d, vec.d, MM_SHUFFLE_SWIZZLE_HELPER( at, bt, ct, 0 ) ) );
      return *( vec3i<float>* )this;
    }

    template<int at, int bt, int ct, int dt>
    const vec4i<float>& vec4i<float>::swizzle<at, bt, ct, dt>::operator/=( const vec4i<float>& vec )
    {
      assert( all( notEqual( vec, vec4i<float>( 0 ) ) ) );
      vec4i<float>* tmp = (vec4i<float>*)this;
      tmp->d = _mm_div_ps( v, _mm_shuffle_ps( vec.d, vec.d, MM_SHUFFLE_SWIZZLE_HELPER( at, bt, ct, dt ) ) );
      return *( vec4i<float>* )this;
    }
#endif

    const vec2i<float>& vec2i<float>::operator/=( const vec2i<float>& vec )
    {
      assert( all( notEqual( vec, vec2i<float>( 0 ) ) ) );
      vec2i<float>* tmp = (vec2i<float>*)this;
      tmp->d = _mm_div_ps( tmp->d, vec.d );
      return *this;
    }

    const vec3i<float>& vec3i<float>::operator/=( const vec3i<float>& vec )
    {
      assert( all( notEqual( vec, vec3i<float>( 0 ) ) ) );
      vec3i<float>* tmp = (vec3i<float>*)this;
      tmp->d = _mm_div_ps( tmp->d, vec.d );
      return *this;
    }

    const vec4i<float>& vec4i<float>::operator/=( const vec4i<float>& vec )
    {
      assert( all( notEqual( vec, vec4i<float>( 0 ) ) ) );
      vec4i<float>* tmp = (vec4i<float>*)this;
      tmp->d = _mm_div_ps( tmp->d, vec.d );
      return *this;
    }
  }
}

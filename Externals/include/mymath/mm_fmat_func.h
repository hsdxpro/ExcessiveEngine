#ifndef mm_mat_func_h
#define mm_mat_func_h

#include "mm_common.h"

#ifdef MYMATH_USE_SSE2
#include "mm_fvec_func.h"
#endif

#include "mm_mat2_impl.h"
#include "mm_mat3_impl.h"
#include "mm_mat4_impl.h"

template< typename t >
MYMATH_INLINE std::ostream& operator<< ( std::ostream& output, const mm::impl::mat2i<t>& mat )
{
  return output << "( " << mat[0].x << ", " << mat[1].x << "\n  "
         /*__________*/ << mat[0].y << ", " << mat[1].y << " )\n";
}

template< typename t >
MYMATH_INLINE std::ostream& operator<< ( std::ostream& output, const mm::impl::mat3i<t>& mat )
{
  return output << "( " << mat[0].x << ", " << mat[1].x << ", " << mat[2].x << "\n  "
         /*__________*/ << mat[0].y << ", " << mat[1].y << ", " << mat[2].y << "\n  "
         /*__________*/ << mat[0].z << ", " << mat[1].z << ", " << mat[2].z << " )\n";
}

template< typename t >
MYMATH_INLINE std::ostream& operator<< ( std::ostream& output, const mm::impl::mat4i<t>& mat )
{
  return output << "( " << mat[0].x << ", " << mat[1].x << ", " << mat[2].x << ", " << mat[3].x << "\n  "
         /*__________*/ << mat[0].y << ", " << mat[1].y << ", " << mat[2].y << ", " << mat[3].y << "\n  "
         /*__________*/ << mat[0].z << ", " << mat[1].z << ", " << mat[2].z << ", " << mat[3].z << "\n  "
         /*__________*/ << mat[0].w << ", " << mat[1].w << ", " << mat[2].w << ", " << mat[3].w << " )\n";
}

namespace mymath
{
  template< typename t >
  MYMATH_INLINE impl::mat2i<t> transpose( const impl::mat2i<t>& mat )
  {
    impl::vec4i<t> tmp1 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(0, 1, 0, 1) );
    return impl::mat2i<t>( tmp1.xz, tmp1.yw );
  }

  template< typename t >
  MYMATH_INLINE impl::mat3i<t> transpose( const impl::mat3i<t>& mat )
  {
    impl::vec4i<t> tmp1 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(0, 1, 0, 1) ); //adbe
    impl::vec4i<t> tmp2 = mat[2].xyzz;                                                  //cfii
    impl::vec4i<t> tmp3 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(2, 2, 2, 2) ); //gghh

    return impl::mat3i<t>( _mm_shuffle_ps( tmp1.d, tmp2.d, MYMATH_SHUFFLE( 0, 2, 0, 0 ) ), 
                           _mm_shuffle_ps( tmp1.d, tmp2.d, MYMATH_SHUFFLE( 1, 3, 1, 1 ) ),
                           _mm_shuffle_ps( tmp3.d, tmp2.d, MYMATH_SHUFFLE( 0, 2, 2, 3 ) ) );
  }

  template< typename t >
  MYMATH_INLINE impl::mat4i<t> transpose( const impl::mat4i<t>& mat )
  {
    impl::vec4i<t> tmp1 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(0, 1, 0, 1) );
    impl::vec4i<t> tmp2 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(2, 3, 2, 3) );
    impl::vec4i<t> tmp3 = _mm_shuffle_ps( mat[2].d, mat[3].d, MYMATH_SHUFFLE(0, 1, 0, 1) );
    impl::vec4i<t> tmp4 = _mm_shuffle_ps( mat[2].d, mat[3].d, MYMATH_SHUFFLE(2, 3, 2, 3) );

    return impl::mat4i<t>( _mm_shuffle_ps( tmp1.d, tmp3.d, MYMATH_SHUFFLE(0, 2, 0, 2) ),
                           _mm_shuffle_ps( tmp1.d, tmp3.d, MYMATH_SHUFFLE(1, 3, 1, 3) ),
                           _mm_shuffle_ps( tmp2.d, tmp4.d, MYMATH_SHUFFLE(0, 2, 0, 2) ),
                           _mm_shuffle_ps( tmp2.d, tmp4.d, MYMATH_SHUFFLE(1, 3, 1, 3) ) );
  }
}

#define MYMATH_VECMULMAT_FUNC(t) \
  MYMATH_INLINE mm::impl::vec2i<t> operator*( const mm::impl::vec2i<t>& vec, const mm::impl::mat2i<t>& mat ) \
  { \
    mm::impl::mat2i<t> tmp = mm::transpose( mat ); \
    return tmp * vec; \
  } \
  MYMATH_INLINE mm::impl::vec3i<t> operator*( const mm::impl::vec3i<t>& vec, const mm::impl::mat3i<t>& mat ) \
  { \
    mm::impl::mat3i<t> tmp = mm::transpose( mat ); \
    return tmp * vec; \
  } \
  MYMATH_INLINE mm::impl::vec4i<t> operator*( const mm::impl::vec4i<t>& vec, const mm::impl::mat4i<t>& mat ) \
  { \
    mm::impl::mat4i<t> tmp = mm::transpose( mat ); \
    return tmp * vec; \
  }

#define MYMATH_MATMULVEC_FUNC(t) \
  MYMATH_INLINE mm::impl::vec2i<t> operator* ( const mm::impl::mat2i<t>& mat, const mm::impl::vec2i<t>& vec ) \
  { \
    return mm::fma( vec.yy, mat[1], vec.xx * mat[0] ); \
  } \
  MYMATH_INLINE mm::impl::vec3i<t> operator* ( const mm::impl::mat3i<t>& mat, const mm::impl::vec3i<t>& vec ) \
  { \
    return mm::fma( vec.zzz, mat[2], mm::fma( vec.yyy, mat[1], vec.xxx * mat[0] ) ); \
  } \
  MYMATH_INLINE mm::impl::vec4i<t> operator*( const mm::impl::mat4i<t>& mat, const mm::impl::vec4i<t>& vec ) \
  { \
    return mm::fma( vec.wwww, mat[3], mm::fma( vec.zzzz, mat[2], mm::fma( vec.yyyy, mat[1], vec.xxxx * mat[0] ) ) ); \
  }

#define MYMATH_VECMULEQUALMAT_FUNC(t) \
  MYMATH_INLINE const mm::impl::vec2i<t>& operator*=( mm::impl::vec2i<t>& vec, const mm::impl::mat2i<t>& mat ) \
  { \
    mm::impl::vec2i<t> res = vec * mat; \
    vec = res; \
    return vec; \
  } \
  MYMATH_INLINE const mm::impl::vec3i<t>& operator*=( mm::impl::vec3i<t>& vec, const mm::impl::mat3i<t>& mat ) \
  { \
    mm::impl::vec3i<t> res = vec * mat; \
    vec = res; \
    return vec; \
  } \
  MYMATH_INLINE const mm::impl::vec4i<t>& operator*=( mm::impl::vec4i<t>& vec, const mm::impl::mat4i<t>& mat ) \
  { \
    mm::impl::vec4i<t> res = vec * mat; \
    vec = res; \
    return vec; \
  }

template< typename t >
MYMATH_INLINE mm::impl::mat2i<t> operator* ( const mm::impl::mat2i<t>& a, const mm::impl::mat2i<t>& b )
{
  mm::impl::vec2i<t> tmp1 = a[0];
  mm::impl::vec2i<t> tmp2 = a[1];

  return mm::impl::mat2i<t>( mm::fma(b[0].xx, tmp1, b[0].yy * tmp2), 
                             mm::fma(b[1].xx, tmp1, b[1].yy * tmp2) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat3i<t> operator* ( const mm::impl::mat3i<t>& a, const mm::impl::mat3i<t>& b )
{

  mm::impl::vec3i<t> tmp1 = a[0];
  mm::impl::vec3i<t> tmp2 = a[1];
  mm::impl::vec3i<t> tmp3 = a[2];

  return mm::impl::mat3i<t>( mm::fma(b[0].zzz, tmp3, mm::fma(b[0].yyy, tmp2, b[0].xxx * tmp1)),
    mm::fma( b[1].zzz, tmp3, mm::fma( b[1].yyy, tmp2, b[1].xxx * tmp1 ) ),
    mm::fma( b[2].zzz, tmp3, mm::fma( b[2].yyy, tmp2, b[2].xxx * tmp1 ) ) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat4i<t> operator* ( const mm::impl::mat4i<t>& a, const mm::impl::mat4i<t>& b )
{
  mm::impl::vec4i<t> tmp1 = a[0];
  mm::impl::vec4i<t> tmp2 = a[1];
  mm::impl::vec4i<t> tmp3 = a[2];
  mm::impl::vec4i<t> tmp4 = a[3];

  return mm::impl::mat4i<t>( mm::fma(b[0].wwww, tmp4, mm::fma(b[0].zzzz, tmp3, mm::fma(b[0].yyyy, tmp2, b[0].xxxx * tmp1))),
                             mm::fma(b[1].wwww, tmp4, mm::fma(b[1].zzzz, tmp3, mm::fma(b[1].yyyy, tmp2, b[1].xxxx * tmp1))),
                             mm::fma(b[2].wwww, tmp4, mm::fma(b[2].zzzz, tmp3, mm::fma(b[2].yyyy, tmp2, b[2].xxxx * tmp1))),
                             mm::fma(b[3].wwww, tmp4, mm::fma(b[3].zzzz, tmp3, mm::fma(b[3].yyyy, tmp2, b[3].xxxx * tmp1))) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat2i<t> operator*( const mm::impl::mat2i<t>& mat, const t& num )
{
  return mm::impl::mat2i<t>( mat[0] * mm::impl::vec2i<t>( num ), mat[1] * mm::impl::vec2i<t>( num ) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat3i<t> operator*( const mm::impl::mat3i<t>& mat, const t& num )
{
  return mm::impl::mat3i<t>( mat[0] * mm::impl::vec3i<t>( num ), mat[1] * mm::impl::vec3i<t>( num ), mat[2] * mm::impl::vec3i<t>( num ) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat4i<t> operator*( const mm::impl::mat4i<t>& mat, const t& num )
{
  return mm::impl::mat4i<t>( mat[0] * mm::impl::vec4i<t>( num ), mat[1] * mm::impl::vec4i<t>( num ), mat[2] * mm::impl::vec4i<t>( num ), mat[3] * mm::impl::vec4i<t>( num ) );
}

template< typename t >
MYMATH_INLINE mm::impl::mat2i<t> operator*( const t& num, const mm::impl::mat2i<t>& mat )
{
  return mat * num;
}

template< typename t >
MYMATH_INLINE mm::impl::mat3i<t> operator*( const t& num, const mm::impl::mat3i<t>& mat )
{
  return mat * num;
}

template< typename t >
MYMATH_INLINE mm::impl::mat4i<t> operator*( const t& num, const mm::impl::mat4i<t>& mat )
{
  return mat * num;
}

MYMATH_MATMULVEC_FUNC( float )
MYMATH_VECMULMAT_FUNC( float )

MYMATH_VECMULEQUALMAT_FUNC( float )

#if MYMATH_DOUBLE_PRECISION == 1
MYMATH_VECMULMAT_FUNC( double )
MYMATH_MATMULVEC_FUNC( double )
#endif

namespace mymath
{
  template< typename t >
  MYMATH_INLINE impl::vec4i<t> determinant_helper( const impl::mat2i<t>& mat )
  {
    impl::vec2i<t> tmp = mat[0] * mat[1].yx;
    return ( tmp - tmp.yx ).xxxx;
  }

  template< typename t >
  MYMATH_INLINE t determinant( const impl::mat2i<t>& mat )
  {
    return determinant_helper( mat ).x;
  }

  template< typename t >
  MYMATH_INLINE impl::vec4i<t> determinant_helper( const impl::mat3i<t>& mat )
  {
    impl::vec3i<t> tmp1 = cross( mat[0], mat[1] ); //2 mul, 1 sub, 4 shuffle, 1 mov
    tmp1 *= mat[2]; //1 mul, 1 mov
    return tmp1.xxxx + tmp1.yyyy + tmp1.zzzz; //2 add
  }

  template< typename t >
  MYMATH_INLINE t determinant( const impl::mat3i<t>& mat )
  {
    return determinant_helper( mat ).x;
  }

//helper for _mm_set_ps()
#define MYMATH_SSE_SETTER(x, y, z, w) _mm_set_ps(w, z, y, x)

  template< typename t >
  MYMATH_INLINE impl::vec4i<t> determinant_helper( const impl::mat4i<t>& mat )
  {
    impl::vec4i<t> aa = mat[0].zxxx * mat[1].wyzw; //in, af, aj, an
    impl::vec4i<t> ee = mat[1].zxxx * mat[0].wyzw; //jm, be, bi, bm
    impl::vec4i<t> ii = mat[0].zwyy * mat[1].yyzw; //if, mf, ej, en

    //in - jm, af - be, aj - bi, an - bm
    aa = aa - ee;

    //xx - xx, xx - xx, ej - fi, en - fm
    ii = ii - ii.xxxy;

    //an - bm, aj - bi, en - fm, xx - xx
    impl::vec4i<t> jj = _mm_shuffle_ps(aa.d, ii.d, MYMATH_SHUFFLE(3, 2, 3, 3));
    //-k(en - fm), -k(an - bm), -g(an - bm), -g(aj - bi)
    ee = jj.zxxy * mat[2].zzyy;

    //in - jm, in - jm, en - fm, ej - fi
    jj = _mm_shuffle_ps( aa.d, ii.d, MYMATH_SHUFFLE(0, 0, 3, 2) );
    //g(in - jm), c(in - jm), c(en - fm), c(ej - fi)
    impl::vec4i<t> ww = jj * mat[2].yxxx; 

    //af - be, aj - bi, ej - fi, ej - fi
    jj = _mm_shuffle_ps( aa.d, ii.d, MYMATH_SHUFFLE(1, 2, 2, 2) );
    //o(ej - fi), o(aj - bi), o(af - be), k(af - be)
    jj = jj.zyxx * mat[2].wwwz; 

    jj = mat[3] * MYMATH_SSE_SETTER(-1, 1, -1, 1) * (ww - ee + jj);
    jj = jj + jj.wzyx;   //xw, yz, zy, wx
    jj = jj + jj.zxxz;   //xwzy, yzxw, zyxw, wxzy

    return jj;
  }

  template< typename t >
  MYMATH_INLINE t determinant( const impl::mat4i<t>& mat )
  {
    return determinant_helper( mat ).x;
  }

  template< typename t >
  MYMATH_INLINE impl::mat2i<t> inverse( const impl::mat2i<t>& mat )
  {
    assert( determinant( mat ) != 0 );
    impl::vec4i<t> tmp1 = _mm_shuffle_ps( mat[0].d, mat[1].d, MYMATH_SHUFFLE(0, 1, 0, 1));
    impl::vec4i<t> det = _mm_div_ps( impl::one, determinant_helper( mat ).d );
    tmp1 = tmp1 * MYMATH_SSE_SETTER(1, -1, -1, 1) * det;
    return impl::mat2i<t>( tmp1.wy, tmp1.zx );
  }

  //middle row/column is not good
  template< typename t >
  MYMATH_INLINE impl::mat3i<t> inverse( const impl::mat3i<t>& mat )
  {
    assert( determinant( mat ) != 0 );

    impl::vec4i<t> atmp2 = mat[1].zzyy * mat[2].yxxy; //h*f, h*c, e*c, e*f
    //ebb * iif - hhe * fcc
    impl::vec4i<t> atmp1 = impl::sse_fms_ps( impl::vec4i<t>(mat[1].yxxy).d, impl::vec4i<t>(mat[2].zzyy).d, atmp2.d );

    impl::vec4i<t> atmp4 = mat[0].zzyy * mat[2].yxxy; //g*f, g*c, d*c, d*f
    //daa * iif - ggd * fcc
    impl::vec4i<t> atmp3 = impl::sse_fms_ps( impl::vec4i<t>(mat[0].yxxy).d,  impl::vec4i<t>(mat[2].zzyy).d, atmp4.d );
    
    impl::vec4i<t> atmp6 = mat[0].zzyy * mat[1].yxxy; //g*e, g*b, d*b, d*e
    //daa * hhe - ggd * ebb
    impl::vec4i<t> atmp5 = impl::sse_fms_ps( impl::vec4i<t>(mat[0].yxxy).d, impl::vec4i<t>(mat[1].zzyy).d, atmp6.d );

    atmp1 *= MYMATH_SSE_SETTER(1, -1, 1, 1);
    atmp3 *= MYMATH_SSE_SETTER(-1, 1, -1, 1);
    atmp5 *= MYMATH_SSE_SETTER(1, -1, 1, 1);

    impl::mat3i<t> ret( atmp1.xyz, atmp3.xyz, atmp5.xyz );
    ret = transpose( ret );

    impl::vec4i<t> adet = _mm_div_ps( impl::one, determinant_helper( mat ).d );

    ret[0] *= adet.xxx;
    ret[1] *= adet.xxx;
    ret[2] *= adet.xxx;

    return ret;
  }

  //TODO optimize this
  template< typename t >
  MYMATH_INLINE impl::mat4i<t> inverse( const impl::mat4i<t>& mat )
  {
    assert( determinant( mat ) != 0 );

    impl::vec4i<t> pmpm = MYMATH_SSE_SETTER(1, -1, 1, -1);
    impl::vec4i<t> mpmp = MYMATH_SSE_SETTER(-1, 1, -1, 1);

    mat4 m = transpose(mat);

    impl::vec4i<t> shf0yxxx = m[0].yxxx;
    impl::vec4i<t> shf0wwwz = m[0].wwwz;
    impl::vec4i<t> shf0zzyy = m[0].zzyy;

    impl::vec4i<t> shf1yxxx = m[1].yxxx;
    impl::vec4i<t> shf1wwwz = m[1].wwwz;
    impl::vec4i<t> shf1zzyy = m[1].zzyy;
    
    impl::vec4i<t> shf2yxxx = m[2].yxxx;
    impl::vec4i<t> shf2wwwz = m[2].wwwz;
    impl::vec4i<t> shf2zzyy = m[2].zzyy;

    impl::vec4i<t> shf3yxxx = m[3].yxxx;
    impl::vec4i<t> shf3wwwz = m[3].wwwz;
    impl::vec4i<t> shf3zzyy = m[3].zzyy;

    impl::vec4i<t> mul0 = shf0zzyy * shf1wwwz; 
    impl::vec4i<t> mul1 = shf0yxxx * shf1wwwz; 

    impl::vec4i<t> mul2 = shf2wwwz * shf3yxxx; 
    impl::vec4i<t> mul3 = shf2wwwz * shf3zzyy; 

    impl::vec4i<t> mul4 = shf0yxxx * shf1zzyy; 
    impl::vec4i<t> mul5 = shf0wwwz * shf1zzyy; 

    impl::vec4i<t> mul6 = shf0wwwz * shf1yxxx; 
    impl::vec4i<t> mul7 = shf0zzyy * shf1yxxx; 

    impl::vec4i<t> atmp3 = mul0 * shf2yxxx;
    impl::vec4i<t> atmp6 = mul1 * shf2zzyy;

    impl::vec4i<t> atmp12 = mul0 * shf3yxxx;
    impl::vec4i<t> atmp15 = mul1 * shf3zzyy;

    impl::vec4i<t> atmp21 = shf0zzyy * mul2;
    impl::vec4i<t> atmp24 = shf0yxxx * shf2wwwz * shf3zzyy;

    impl::vec4i<t> atmp30 = shf1zzyy * mul2;
    impl::vec4i<t> atmp33 = shf1yxxx * shf2wwwz * shf3zzyy;

    impl::vec4i<t> atmp7 = fma( mul4, shf2wwwz, fma( mul6, shf2zzyy, atmp3 ) );
    impl::vec4i<t> atmp8 = fma( mul5, shf2yxxx, fma( mul7, shf2wwwz, atmp6 ) );

    impl::vec4i<t> atmp16 = fma( mul4, shf3wwwz, fma( mul6, shf3zzyy, atmp12 ) );
    impl::vec4i<t> atmp17 = fma( mul5, shf3yxxx, fma( mul7, shf3wwwz, atmp15 ) );

    impl::vec4i<t> atmp25 = fma( shf0yxxx * shf2zzyy, shf3wwwz, fma( shf0wwwz * shf2yxxx, shf3zzyy, atmp21 ) );
    impl::vec4i<t> atmp26 = fma( shf0wwwz * shf2zzyy, shf3yxxx, fma( shf0zzyy * shf2yxxx, shf3wwwz, atmp24 ) );

    impl::vec4i<t> atmp34 = fma( shf1yxxx * shf2zzyy, shf3wwwz, fma( shf1wwwz * shf2yxxx, shf3zzyy, atmp30 ) );
    impl::vec4i<t> atmp35 = fma( shf1wwwz * shf2zzyy, shf3yxxx, fma( shf1zzyy * shf2yxxx, shf3wwwz, atmp33 ) );

    //dhlp
    impl::vec4i<t> atmp9 = (atmp7 - atmp8) * mpmp;

    //cgko
    impl::vec4i<t> atmp18 = (atmp16 - atmp17) * pmpm; 

    //bfjn
    impl::vec4i<t> atmp27 = (atmp25 - atmp26) * mpmp;

    //aeim
    impl::vec4i<t> atmp36 = (atmp34 - atmp35) * pmpm;

    impl::mat4i<t> ret( atmp36, atmp27, atmp18, atmp9 );

    impl::vec4i<t> adet = _mm_div_ps( impl::one, determinant_helper( m ).d );

    ret[0] *= adet;
    ret[1] *= adet;
    ret[2] *= adet;
    ret[3] *= adet;

    return ret;
  }

  template< typename t >
  MYMATH_INLINE impl::mat2i<t> matrixCompMult( const impl::mat2i<t>& a, const impl::mat2i<t>& b )
  {
    return impl::mat2i<t>( a[0] * b[0], a[1] * b[1] );
  }

  template< typename t >
  MYMATH_INLINE impl::mat3i<t> matrixCompMult( const impl::mat3i<t>& a, const impl::mat3i<t>& b )
  {
    return impl::mat3i<t>( a[0] * b[0], a[1] * b[1], a[2] * b[2] );
  }

  template< typename t >
  impl::mat4i<t> matrixCompMult( const impl::mat4i<t>& a, const impl::mat4i<t>& b )
  {
    return impl::mat4i<t>( a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] );
  }
}

#endif




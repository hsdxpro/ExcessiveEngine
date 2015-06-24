#ifndef mm_vec_swizzle_out_h
#define mm_vec_swizzle_out_h

#include "mm_common.h"
#include "mm_vec2_impl.h"
#include "mm_vec3_impl.h"
#include "mm_vec4_impl.h"

namespace mymath
{
  namespace impl
  {
    //vec3 swizzlers for vec2
    template<typename ty>
    template<int ta>
    class vec2i<ty>::swizzle < ta, ta, ta, -3 >
    {
      private:
        ty v[2];
      public:
        operator vec3i<ty>() const
        {
          return vec3i<ty>( v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle < tb, ta, ta, -3 >
    {
      private:
        ty v[2];
      public:
        operator vec3i<ty>() const
        {
          return vec3i<ty>( v[tb], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle < ta, tb, ta, -3 >
    {
      private:
        ty v[2];
      public:
        operator vec3i<ty>() const
        {
          return vec3i<ty>( v[ta], v[tb], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle < ta, ta, tb, -3 >
    {
      private:
        ty v[2];
      public:
        operator vec3i<ty>() const
        {
          return vec3i<ty>( v[ta], v[ta], v[tb] );
        }
    };

    //vec4 swizzlers for vec2
    template<typename ty>
    template<int ta>
    class vec2i<ty>::swizzle<ta, ta, ta, ta>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<tb, ta, ta, ta>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[tb], v[ta], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<ta, tb, ta, ta>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[tb], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<ta, ta, tb, ta>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[ta], v[tb], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<ta, ta, ta, tb>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[ta], v[ta], v[tb] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<ta, tb, ta, tb>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[tb], v[ta], v[tb] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec2i<ty>::swizzle<ta, ta, tb, tb>
    {
      private:
        ty v[2];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[ta], v[tb], v[tb] );
        }
    };

    //vec4 swizzlers for vec3
    template<typename ty>
    template<int ta>
    class vec3i<ty>::swizzle<ta, ta, ta, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec3i<ty>::swizzle<tb, ta, ta, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[tb], v[ta], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec3i<ty>::swizzle<ta, tb, ta, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[tb], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec3i<ty>::swizzle<ta, ta, tb, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[ta], v[tb], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb>
    class vec3i<ty>::swizzle<ta, ta, ta, tb>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[ta], v[ta], v[ta], v[tb] );
        }
    };

    template<typename ty>
    template<int ta, int tb, int tc>
    class vec3i<ty>::swizzle<tb, tc, ta, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[tb], v[tc], v[ta], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb, int tc>
    class vec3i<ty>::swizzle<tb, ta, tc, ta>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[tb], v[ta], v[tc], v[ta] );
        }
    };

    template<typename ty>
    template<int ta, int tb, int tc>
    class vec3i<ty>::swizzle<tb, ta, ta, tc>
    {
      private:
        ty v[3];
      public:
        operator vec4i<ty>() const
        {
          return vec4i<ty>( v[tb], v[ta], v[ta], v[tc] );
        }
    };
  }
}

#endif

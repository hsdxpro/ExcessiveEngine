#ifndef random_h
#define random_h

template< class t >
class random_wrapper
{
  public:
    static void init()
    {
      t::init();
    }

    static f32 get( f32 min, f32 max )
    {
      return min + ( max - min ) * t::get() / ( f32 )RAND_MAX;
    }
};

#endif

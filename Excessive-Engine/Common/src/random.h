#ifndef Random_h
#define Random_h

template< class t >
class RandomWrapper
{
  public:
    static void init()
    {
      t::init();
    }

    static float get( float min, float max )
    {
      return min + ( max - min ) * t::get() / ( float )RAND_MAX;
    }
};

#endif

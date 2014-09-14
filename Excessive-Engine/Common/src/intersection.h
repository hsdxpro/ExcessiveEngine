#ifndef intersection_h
#define intersection_h

#include "mymath/mymath.h"
#include <vector>
#include <float.h>

//based on the fast, constant time multi-dispatcher design pattern
template<class lhs, 
class rhs = lhs, 
class ret = void,
class cbk = ret (*)(lhs, rhs)>
class Dispatcher
{
  typedef std::vector<cbk> row;
  typedef std::vector<row> matrix;
  matrix callbacks;
  int columns;
public:
  Dispatcher() : columns(0){}

  template<class _lhs, class _rhs>
  void add(cbk func)
  {
    int& idx_lhs = _lhs::getClassIdx();

    if(idx_lhs < 0) //not set
    {
      callbacks.push_back(row());
      idx_lhs = callbacks.size() - 1;
    }
    else if((int)callbacks.size() <= idx_lhs)
    {
      callbacks.resize(idx_lhs + 1);
    }

    row& cur_row = callbacks[idx_lhs];
    int& idx_rhs = _rhs::getClassIdx();

    if(idx_rhs < 0) //not set
    {
      cur_row.resize(++columns);
      idx_rhs = cur_row.size() - 1;
    }
    else if((int)cur_row.size() <= idx_rhs)
    {
      cur_row.resize(idx_rhs + 1);
    }

    cur_row[idx_rhs] = func;
  }

  ret go(lhs _lhs, rhs _rhs)
  {
    int idx_lhs = _lhs->getClassIndex();
    int idx_rhs = _rhs->getClassIndex();

    assert( idx_lhs >= 0 || idx_rhs >=  0 || idx_lhs < (int)callbacks.size() || idx_rhs < (int)callbacks.size() );

    return callbacks[idx_lhs][idx_rhs](_lhs, _rhs);
  }
};

//forward declarations
class Sphere;
class Plane;
class Aabb;
class Frustum;

//generic abstract Shape class
//needed so that any Shape can intersect any other Shape
class Shape
{
  static Dispatcher<Shape*, Shape*, bool> _is_on_right_side;
  static Dispatcher<Shape*, Shape*, bool> _is_inside;
  static Dispatcher<Shape*, Shape*, bool> _intersects;
  static bool is_setup;
public:
  static void setUpIntersection();

  bool isOnRightSide(Shape* s)
  {
    assert(is_setup);
    return _is_on_right_side.go(this, s);
  }

  bool isInside(Shape* s)
  {
    assert(is_setup);
    return _is_inside.go(this, s);
  }

  bool intersects(Shape* s)
  {
    assert(is_setup);
    return _intersects.go(this, s);
  }

  virtual int getClassIndex() = 0;
};

Dispatcher<Shape*, Shape*, bool> Shape::_is_on_right_side;
Dispatcher<Shape*, Shape*, bool> Shape::_is_inside;
Dispatcher<Shape*, Shape*, bool> Shape::_intersects;
bool Shape::is_setup = false;

class Sphere : public Shape
{
public:
  //define a Sphere by a center and a radius
  mm::vec3 center;
  float radius;

  static int& getClassIdx()
  {
    static int idx = -1;
    return idx;
  }

  int getClassIndex()
  {
    return getClassIdx();
  }

  Sphere( mm::vec3 c = mm::vec3(), float r = float() ) : center( c ), radius( r ) {}
};

class Plane : public Shape
{
public:
  //define a Plane by a normal and a point
  mm::vec3 normal, point;
  float d; //cache -(normal dot point)

  static int& getClassIdx()
  {
    static int idx = -1;
    return idx;
  }

  int getClassIndex()
  {
    return getClassIdx();
  }

  //define a Plane by 3 points
  void setUp( const mm::vec3& a, const mm::vec3& b, const mm::vec3& c )
  {
    mm::vec3 tmp1, tmp2;

    tmp1 = a - b;
    tmp2 = c - b;

    normal = mm::normalize( mm::cross( tmp2, tmp1 ) );
    point = a;
    d = -mm::dot( normal, point );
  }

  //signed distance
  float distance( const mm::vec3& p )
  {
    return d + mm::dot( normal, p );
  }

  //define a Plane by a normal and a point
  Plane( const mm::vec3& n = mm::vec3(), const mm::vec3& p = mm::vec3() ) : normal( n ), point( p )
  {
    d = -mm::dot( n, p );
  }

  Plane( const mm::vec3& a, const mm::vec3& b, const mm::vec3& c )
  {
    setUp( a, b, c );
  }
};

class Aabb : public Shape
{
public:
  mm::vec3 pos; //center of the Aabb
  mm::vec3 extents; //half-width/height of the Aabb
  mm::vec3 min, max; //minimum/maximum apex of the Aabb

  static int& getClassIdx()
  {
    static int idx = -1;
    return idx;
  }

  int getClassIndex()
  {
    return getClassIdx();
  }

  //returns the vertices of the triangles of the Aabb in counter clockwise order
  std::vector<mm::vec3> getVertices()
  {
    std::vector<mm::vec3> v;

    //left
    v.push_back( mm::vec3( min.x, max.yz ) );
    v.push_back( mm::vec3( min.x, max.y, min.z ) );
    v.push_back( mm::vec3( min.xyz ) );

    v.push_back( mm::vec3( min.xyz ) );
    v.push_back( mm::vec3( min.xy, max.z ) );
    v.push_back( mm::vec3( min.x, max.yz ) );

    //front
    v.push_back( mm::vec3( min.xy, max.z ) );
    v.push_back( mm::vec3( max.x, min.y, max.z ) );
    v.push_back( mm::vec3( max.xyz ) );

    v.push_back( mm::vec3( max.xyz ) );
    v.push_back( mm::vec3( min.x, max.yz ) );
    v.push_back( mm::vec3( min.xy, max.z ) );

    //right
    v.push_back( mm::vec3( max.xy, min.z ) );
    v.push_back( mm::vec3( max.xyz ) );
    v.push_back( mm::vec3( max.x, min.y, max.z ) );

    v.push_back( mm::vec3( max.x, min.y, max.z ) );
    v.push_back( mm::vec3( max.x, min.yz ) );
    v.push_back( mm::vec3( max.xy, min.z ) );

    //back
    v.push_back( mm::vec3( max.xy, min.z ) );
    v.push_back( mm::vec3( max.x, min.yz ) );
    v.push_back( mm::vec3( min.xyz ) );

    v.push_back( mm::vec3( min.xyz ) );
    v.push_back( mm::vec3( min.x, max.y, min.z ) );
    v.push_back( mm::vec3( max.xy, min.z ) );

    //top
    v.push_back( mm::vec3( min.x, max.y, min.z ) );
    v.push_back( mm::vec3( min.x, max.yz ) );
    v.push_back( mm::vec3( max.xyz ) );

    v.push_back( mm::vec3( max.xyz ) );
    v.push_back( mm::vec3( max.xy, min.z ) );
    v.push_back( mm::vec3( min.x, max.y, min.z ) );

    //bottom
    v.push_back( mm::vec3( max.x, min.y, max.z ) );
    v.push_back( mm::vec3( min.xy, max.z ) );
    v.push_back( mm::vec3( min.xyz ) );

    v.push_back( mm::vec3( min.xyz ) );
    v.push_back( mm::vec3( max.x, min.yz ) );
    v.push_back( mm::vec3( max.x, min.y, max.z ) );

    return v;
  }

  mm::vec3 getPosVertex( const mm::vec3& n )
  {
    mm::vec3 res = min;

    if( n.x >= 0 )
      res.x = max.x;

    if( n.y >= 0 )
      res.y = max.y;

    if( n.z >= 0 )
      res.z = max.z;

    return res;
  }

  void expand( const mm::vec3& p )
  {
    min = mm::min( min, p );
    max = mm::max( max, p );
    extents = ( max - min ) / 2.0f;
    pos = min + extents;
  }

  mm::vec3 getNegVertex( const mm::vec3& n )
  {
    mm::vec3 res = max;

    if( n.x >= 0 )
      res.x = min.x;

    if( n.y >= 0 )
      res.y = min.y;

    if( n.z >= 0 )
      res.z = min.z;

    return res;
  }

  void resetMinmax()
  {
    min = mm::vec3( FLT_MAX );
    max = mm::vec3( -FLT_MAX );
  }

  Aabb()
  {
    resetMinmax();
  }

  Aabb( const mm::vec3& p, const mm::vec3& e ) : pos( p ), extents( e )
  {
    min = pos - extents;
    max = pos + extents;
  }
};

//haxx
#ifdef WIN_BUILD
#undef FAR
#endif

class Frustum : public Shape
{
public:
  Shape* planes[6];
  mm::vec3 points[8];

  enum eWhichPlane
  {
    TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR
  };

  enum eWhichPoint
  {
    NTL = 0, NTR, NBL, NBR, FTL, FTR, FBL, FBR
  };

  static int& getClassIdx()
  {
    static int idx = -1;
    return idx;
  }

  int getClassIndex()
  {
    return getClassIdx();
  }

  Frustum()
  {
    for( int c = 0; c < 6; ++c )
      planes[c] = new Plane();
  }

  void setUp( const mm::camera<float>& cam )
  {
    mm::vec3 nc = cam.pos - cam.view_dir * cam.get_frame()->near_ll.z;
    mm::vec3 fc = cam.pos - cam.view_dir * cam.get_frame()->far_ll.z;

    mm::vec3 right = -mm::normalize( mm::cross( cam.up_vector, cam.view_dir ) );

    float nw = cam.get_frame()->near_lr.x - cam.get_frame()->near_ll.x;
    float nh = cam.get_frame()->near_ul.y - cam.get_frame()->near_ll.y;

    float fw = cam.get_frame()->far_lr.x - cam.get_frame()->far_ll.x;
    float fh = cam.get_frame()->far_ul.y - cam.get_frame()->far_ll.y;

    //near top left
    mm::vec3 ntl = nc + cam.up_vector * nh - right * nw;
    mm::vec3 ntr = nc + cam.up_vector * nh + right * nw;
    mm::vec3 nbl = nc - cam.up_vector * nh - right * nw;
    mm::vec3 nbr = nc - cam.up_vector * nh + right * nw;

    mm::vec3 ftl = fc + cam.up_vector * fh - right * fw;
    mm::vec3 ftr = fc + cam.up_vector * fh + right * fw;
    mm::vec3 fbl = fc - cam.up_vector * fh - right * fw;
    mm::vec3 fbr = fc - cam.up_vector * fh + right * fw;

    points[NTL] = ntl;
    points[NTR] = ntr;
    points[NBL] = nbl;
    points[NBR] = nbr;

    points[FTL] = ftl;
    points[FTR] = ftr;
    points[FBL] = fbl;
    points[FBR] = fbr;

    static_cast<Plane*>( planes[TOP] )->setUp( ntr, ntl, ftl );
    static_cast<Plane*>( planes[BOTTOM] )->setUp( nbl, nbr, fbr );
    static_cast<Plane*>( planes[LEFT] )->setUp( ntl, nbl, fbl );
    static_cast<Plane*>( planes[RIGHT] )->setUp( nbr, ntr, fbr );
    static_cast<Plane*>( planes[NEAR] )->setUp( ntl, ntr, nbr );
    static_cast<Plane*>( planes[FAR] )->setUp( ftr, ftl, fbl );
  }
};

namespace inner
{
  //only tells if the Sphere is on the right side of the Plane!
  static bool isOnRightSideSP( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Sphere*>(aa);
    auto b = static_cast<Plane*>(bb);

    float dist = b->distance( a->center );
    //dist + radius == how far is the Sphere from the Plane (usable when we want to do lod using the near Plane)

    if( dist < -a->radius )
      return false;

    return true;
  }

  static bool isOnRightSidePS( Shape* aa, Shape* bb )
  {
    return isOnRightSideSP(bb, aa);
  }

  //only tells if the Sphere is on the right side of the Plane!
  static bool isOnRightSideAP( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Plane*>(bb);

    if( b->distance( a->getPosVertex( b->normal ) ) < 0 )
      return false;

    return true;
  }

  static bool isOnRightSidePA( Shape* aa, Shape* bb )
  {
    return isOnRightSideAP(bb, aa);
  }

  static bool intersectSS( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Sphere*>(aa);
    auto b = static_cast<Sphere*>(bb);

    mm::vec3 diff = a->center - b->center;
    float dist = mm::dot( diff, diff );

    float rad_sum = b->radius + a->radius;

    if( dist > rad_sum * rad_sum ) //squared distance check
      return false;

    return true;
  }

  //checks if a Sphere intersects a Plane
  static bool intersectSP( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Sphere*>(aa);
    auto b = static_cast<Plane*>(bb);

    float dist = b->distance( a->center );

    if( abs( dist ) <= a->radius )
      return true;

    return false;
  }

  static bool intersectPS( Shape* aa, Shape* bb )
  {
    return intersectSP(bb, aa);
  }

  static bool intersectPP( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Plane*>(aa);
    auto b = static_cast<Plane*>(bb);

    mm::vec3 vector = mm::cross( a->normal, b->normal );

    //if the cross product yields a null vector
    //then the angle is either 0 or 180
    //that is the two normals are parallel
    // sin(alpha) = 0
    // ==> |a| * |b| * sin(alpha) = 0
    if( mm::equal( vector, mm::vec3( 0 ) ) )
      return false;

    return true;
  }

  static bool intersectAA( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Aabb*>(bb);

    mm::vec3 t = b->pos - a->pos;

    if( abs( t.x ) > ( a->extents.x + b->extents.x ) )
      return false;

    if( abs( t.y ) > ( a->extents.y + b->extents.y ) )
      return false;

    if( abs( t.z ) > ( a->extents.z + b->extents.z ) )
      return false;

    return true;
  }

  static bool intersectAS( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Sphere*>(bb);

    //square distance check between Spheres and Aabbs
    mm::vec3 vec = b->center - mm::clamp( a->pos + (b->center - a->pos), a->min, a->max );

    float sqlength = mm::dot( vec, vec );

    if( sqlength > b->radius * b->radius )
      return false;

    return true;
  }

  static bool intersectSA( Shape* aa, Shape* bb )
  {
    return intersectAS(bb, aa);
  }

  static bool intersectAP( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Plane*>(bb);

    mm::vec3 p = a->getPosVertex( b->normal );
    mm::vec3 n = a->getNegVertex( b->normal );

    float dist_p = b->distance( p );
    float dist_n = b->distance( n );

    if( ( dist_n > 0 && dist_p > 0 ) ||
      ( dist_n < 0 && dist_p < 0 ) )
      return false;

    return true;
  }

  static bool intersectPA( Shape* aa, Shape* bb )
  {
    return intersectAP(bb, aa);
  }

  static bool intersectFS( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Frustum*>(aa);

    for( int c = 0; c < 6; ++c )
    {
      if( !isOnRightSidePS( a->planes[c], bb ) )
        return false;
    }

    return true;
  }

  static bool intersectSF( Shape* aa, Shape* bb )
  {
    return intersectFS(bb, aa);
  }

  static bool intersectFA( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Frustum*>(aa);
    auto b = static_cast<Aabb*>(bb);

    bool res = true;
    for( int c = 0; c < 6; ++c )
    {
      if( !b->isOnRightSide(a->planes[c]) )
      {
        res = false;
        break;
      }
    }
    return res;
  }

  static bool intersectAF( Shape* aa, Shape* bb )
  {
    return intersectFA(bb, aa);
  }

  //is a inside b?
  static bool isInsideSA( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Sphere*>(aa);
    auto b = static_cast<Aabb*>(bb);

    mm::vec3 spheremax = a->center + a->radius;
    mm::vec3 spheremin = a->center - a->radius;

    if( mm::lessThanEqual(spheremax, b->max) && mm::greaterThanEqual(spheremin, b->min) )
      return true;

    return false;
  }

  //is a inside b?
  static bool isInsideAS( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Sphere*>(bb);

    mm::vec3 minvec = a->min - b->center;
    mm::vec3 maxvec = a->max - b->center;
    float sqrmaxlength = mm::dot(maxvec, maxvec);
    float sqrminlength = mm::dot(minvec, minvec);
    float sqrradius = b->radius * b->radius;

    if( sqrmaxlength <= sqrradius && sqrminlength <= sqrradius )
      return true;

    return false;
  }

  //is a inside b?
  static bool isInsideAA( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Aabb*>(aa);
    auto b = static_cast<Aabb*>(bb);

    if( mm::greaterThanEqual( a->min, b->min ) && mm::lessThanEqual( a->max, b->max ) )
      return true;

    return false;
  }

  //is a inside b?
  static bool isInsideSS( Shape* aa, Shape* bb )
  {
    auto a = static_cast<Sphere*>(aa);
    auto b = static_cast<Sphere*>(bb);

    mm::vec3 spheredist = b->center - a->center;

    if( mm::dot(spheredist, spheredist) <= b->radius * b->radius )
      return true;

    return false;
  }
}

void Shape::setUpIntersection()
{
  //order doesnt matter
  _is_on_right_side.add<Sphere, Plane>(inner::isOnRightSideSP);
  _is_on_right_side.add<Plane, Sphere>(inner::isOnRightSidePS);
  _is_on_right_side.add<Aabb, Plane>(inner::isOnRightSideAP);
  _is_on_right_side.add<Plane, Aabb>(inner::isOnRightSidePA);

  _intersects.add<Aabb, Aabb>(inner::intersectAA);
  _intersects.add<Aabb, Plane>(inner::intersectAP);
  _intersects.add<Plane, Aabb>(inner::intersectPA);
  _intersects.add<Aabb, Sphere>(inner::intersectAS);
  _intersects.add<Sphere, Aabb>(inner::intersectSA);
  _intersects.add<Plane, Plane>(inner::intersectPP);
  _intersects.add<Sphere, Plane>(inner::intersectSP);
  _intersects.add<Plane, Sphere>(inner::intersectPS);
  _intersects.add<Sphere, Sphere>(inner::intersectSS);
  _intersects.add<Frustum, Sphere>(inner::intersectFS);
  _intersects.add<Sphere, Frustum>(inner::intersectSF);
  _intersects.add<Frustum, Aabb>(inner::intersectFA);
  _intersects.add<Aabb, Frustum>(inner::intersectAF);

  //order matters
  _is_inside.add<Aabb, Aabb>(inner::isInsideAA);
  _is_inside.add<Aabb, Sphere>(inner::isInsideAS);
  _is_inside.add<Sphere, Aabb>(inner::isInsideSA);
  _is_inside.add<Sphere, Sphere>(inner::isInsideSS);

  //usage
  //is_on_the_right_side.go(new Aabb(), new Sphere());

  is_setup = true;
}

#endif

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
class dispatcher
{
  typedef std::vector<cbk> row;
  typedef std::vector<row> matrix;
  matrix callbacks;
  i32 columns;
public:
  dispatcher() : columns(0){}

  template<class _lhs, class _rhs>
  void add(cbk func)
  {
    i32& idx_lhs = _lhs::get_class_idx();

    if(idx_lhs < 0) //not set
    {
      callbacks.push_back(row());
      idx_lhs = callbacks.size() - 1;
    }
    else if((i32)callbacks.size() <= idx_lhs)
    {
      callbacks.resize(idx_lhs + 1);
    }

    row& cur_row = callbacks[idx_lhs];
    i32& idx_rhs = _rhs::get_class_idx();

    if(idx_rhs < 0) //not set
    {
      cur_row.resize(++columns);
      idx_rhs = cur_row.size() - 1;
    }
    else if((i32)cur_row.size() <= idx_rhs)
    {
      cur_row.resize(idx_rhs + 1);
    }

    cur_row[idx_rhs] = func;
  }

  ret go(lhs _lhs, rhs _rhs)
  {
    i32 idx_lhs = _lhs->get_class_index();
    i32 idx_rhs = _rhs->get_class_index();

    assert( idx_lhs >= 0 || idx_rhs >=  0 || idx_lhs < (i32)callbacks.size() || idx_rhs < (i32)callbacks.size() );

    return callbacks[idx_lhs][idx_rhs](_lhs, _rhs);
  }
};

//forward declarations
class sphere;
class plane;
class aabb;
class frustum;

//generic abstract shape class
//needed so that any shape can intersect any other shape
class shape
{
  static dispatcher<shape*, shape*, bool> _is_on_right_side;
  static dispatcher<shape*, shape*, bool> _is_inside;
  static dispatcher<shape*, shape*, bool> _intersects;
  static bool is_setup;
public:
  static void set_up_intersection();

  bool is_on_right_side(shape* s)
  {
    assert(is_setup);
    return _is_on_right_side.go(this, s);
  }

  bool is_inside(shape* s)
  {
    assert(is_setup);
    return _is_inside.go(this, s);
  }

  bool intersects(shape* s)
  {
    assert(is_setup);
    return _intersects.go(this, s);
  }

  virtual i32 get_class_index() = 0;
};

dispatcher<shape*, shape*, bool> shape::_is_on_right_side;
dispatcher<shape*, shape*, bool> shape::_is_inside;
dispatcher<shape*, shape*, bool> shape::_intersects;
bool shape::is_setup = false;

class sphere : public shape
{
public:
  //define a sphere by a center and a radius
  mm::vec3 center;
  f32 radius;

  static i32& get_class_idx()
  {
    static i32 idx = -1;
    return idx;
  }

  i32 get_class_index()
  {
    return get_class_idx();
  }

  sphere( mm::vec3 c = mm::vec3(), f32 r = f32() ) : center( c ), radius( r ) {}
};

class plane : public shape
{
public:
  //define a plane by a normal and a point
  mm::vec3 normal, point;
  f32 d; //cache -(normal dot point)

  static i32& get_class_idx()
  {
    static i32 idx = -1;
    return idx;
  }

  i32 get_class_index()
  {
    return get_class_idx();
  }

  //define a plane by 3 points
  void set_up( const mm::vec3& a, const mm::vec3& b, const mm::vec3& c )
  {
    mm::vec3 tmp1, tmp2;

    tmp1 = a - b;
    tmp2 = c - b;

    normal = mm::normalize( mm::cross( tmp2, tmp1 ) );
    point = a;
    d = -mm::dot( normal, point );
  }

  //signed distance
  f32 distance( const mm::vec3& p )
  {
    return d + mm::dot( normal, p );
  }

  //define a plane by a normal and a point
  plane( const mm::vec3& n = mm::vec3(), const mm::vec3& p = mm::vec3() ) : normal( n ), point( p )
  {
    d = -mm::dot( n, p );
  }

  plane( const mm::vec3& a, const mm::vec3& b, const mm::vec3& c )
  {
    set_up( a, b, c );
  }
};

class aabb : public shape
{
public:
  mm::vec3 pos; //center of the aabb
  mm::vec3 extents; //half-width/height of the aabb
  mm::vec3 min, max; //minimum/maximum apex of the aabb

  static i32& get_class_idx()
  {
    static i32 idx = -1;
    return idx;
  }

  i32 get_class_index()
  {
    return get_class_idx();
  }

  //returns the vertices of the triangles of the aabb in counter clockwise order
  std::vector<mm::vec3> get_vertices()
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

  mm::vec3 get_pos_vertex( const mm::vec3& n )
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

  mm::vec3 get_neg_vertex( const mm::vec3& n )
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

  void reset_minmax()
  {
    min = mm::vec3( FLT_MAX );
    max = mm::vec3( -FLT_MAX );
  }

  aabb()
  {
    reset_minmax();
  }

  aabb( const mm::vec3& p, const mm::vec3& e ) : pos( p ), extents( e )
  {
    min = pos - extents;
    max = pos + extents;
  }
};

//haxx
#ifdef _WIN32
#undef FAR
#endif

class frustum : public shape
{
public:
  shape* planes[6];
  mm::vec3 points[8];

  enum which_plane
  {
    TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR
  };

  enum which_point
  {
    NTL = 0, NTR, NBL, NBR, FTL, FTR, FBL, FBR
  };

  static i32& get_class_idx()
  {
    static i32 idx = -1;
    return idx;
  }

  i32 get_class_index()
  {
    return get_class_idx();
  }

  frustum()
  {
    for( i32 c = 0; c < 6; ++c )
      planes[c] = new plane();
  }

  void set_up( const mm::camera<f32>& cam )
  {
    mm::vec3 nc = cam.pos - cam.view_dir * cam.get_frame()->near_ll.z;
    mm::vec3 fc = cam.pos - cam.view_dir * cam.get_frame()->far_ll.z;

    mm::vec3 right = -mm::normalize( mm::cross( cam.up_vector, cam.view_dir ) );

    f32 nw = cam.get_frame()->near_lr.x - cam.get_frame()->near_ll.x;
    f32 nh = cam.get_frame()->near_ul.y - cam.get_frame()->near_ll.y;

    f32 fw = cam.get_frame()->far_lr.x - cam.get_frame()->far_ll.x;
    f32 fh = cam.get_frame()->far_ul.y - cam.get_frame()->far_ll.y;

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

    static_cast<plane*>( planes[TOP] )->set_up( ntr, ntl, ftl );
    static_cast<plane*>( planes[BOTTOM] )->set_up( nbl, nbr, fbr );
    static_cast<plane*>( planes[LEFT] )->set_up( ntl, nbl, fbl );
    static_cast<plane*>( planes[RIGHT] )->set_up( nbr, ntr, fbr );
    static_cast<plane*>( planes[NEAR] )->set_up( ntl, ntr, nbr );
    static_cast<plane*>( planes[FAR] )->set_up( ftr, ftl, fbl );
  }
};

namespace inner
{
  //only tells if the sphere is on the right side of the plane!
  static bool is_on_right_side_sp( shape* aa, shape* bb )
  {
    auto a = static_cast<sphere*>(aa);
    auto b = static_cast<plane*>(bb);

    f32 dist = b->distance( a->center );
    //dist + radius == how far is the sphere from the plane (usable when we want to do lod using the near plane)

    if( dist < -a->radius )
      return false;

    return true;
  }

  static bool is_on_right_side_ps( shape* aa, shape* bb )
  {
    return is_on_right_side_sp(bb, aa);
  }

  //only tells if the sphere is on the right side of the plane!
  static bool is_on_right_side_ap( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<plane*>(bb);

    if( b->distance( a->get_pos_vertex( b->normal ) ) < 0 )
      return false;

    return true;
  }

  static bool is_on_right_side_pa( shape* aa, shape* bb )
  {
    return is_on_right_side_ap(bb, aa);
  }

  static bool intersect_ss( shape* aa, shape* bb )
  {
    auto a = static_cast<sphere*>(aa);
    auto b = static_cast<sphere*>(bb);

    mm::vec3 diff = a->center - b->center;
    f32 dist = mm::dot( diff, diff );

    f32 rad_sum = b->radius + a->radius;

    if( dist > rad_sum * rad_sum ) //squared distance check
      return false;

    return true;
  }

  //checks if a sphere intersects a plane
  static bool intersect_sp( shape* aa, shape* bb )
  {
    auto a = static_cast<sphere*>(aa);
    auto b = static_cast<plane*>(bb);

    f32 dist = b->distance( a->center );

    if( abs( dist ) <= a->radius )
      return true;

    return false;
  }

  static bool intersect_ps( shape* aa, shape* bb )
  {
    return intersect_sp(bb, aa);
  }

  static bool intersect_pp( shape* aa, shape* bb )
  {
    auto a = static_cast<plane*>(aa);
    auto b = static_cast<plane*>(bb);

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

  static bool intersect_aa( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<aabb*>(bb);

    mm::vec3 t = b->pos - a->pos;

    if( abs( t.x ) > ( a->extents.x + b->extents.x ) )
      return false;

    if( abs( t.y ) > ( a->extents.y + b->extents.y ) )
      return false;

    if( abs( t.z ) > ( a->extents.z + b->extents.z ) )
      return false;

    return true;
  }

  static bool intersect_as( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<sphere*>(bb);

    //square distance check between spheres and aabbs
    mm::vec3 vec = b->center - mm::clamp( b->center - a->pos, a->min, a->max );

    f32 sqlength = mm::dot( vec, vec );

    if( sqlength > b->radius * b->radius )
      return false;

    return true;
  }

  static bool intersect_sa( shape* aa, shape* bb )
  {
    return intersect_as(bb, aa);
  }

  static bool intersect_ap( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<plane*>(bb);

    mm::vec3 p = a->get_pos_vertex( b->normal );
    mm::vec3 n = a->get_neg_vertex( b->normal );

    f32 dist_p = b->distance( p );
    f32 dist_n = b->distance( n );

    if( ( dist_n > 0 && dist_p > 0 ) ||
      ( dist_n < 0 && dist_p < 0 ) )
      return false;

    return true;
  }

  static bool intersect_pa( shape* aa, shape* bb )
  {
    return intersect_ap(bb, aa);
  }

  static bool intersect_fs( shape* aa, shape* bb )
  {
    auto a = static_cast<frustum*>(aa);

    for( i32 c = 0; c < 6; ++c )
    {
      if( !is_on_right_side_ps( a->planes[c], bb ) )
        return false;
    }

    return true;
  }

  static bool intersect_sf( shape* aa, shape* bb )
  {
    return intersect_fs(bb, aa);
  }

  static bool intersect_fa( shape* aa, shape* bb )
  {
    auto a = static_cast<frustum*>(aa);
    auto b = static_cast<aabb*>(bb);

    /**/
    bool res = true;
    for( i32 c = 0; c < 6; ++c )
    {
      if( !b->is_on_right_side(a->planes[c]) )
      {
        res = false;
        break;
      }
    }
    return res;

    /*vec3 minvec = b->min;
    vec3 maxvec = b->max;

    plane p[6];
    p[0] = plane(vec3(1, 0, 0), minvec);
    p[1] = plane(vec3(0, 1, 0), minvec);
    p[2] = plane(vec3(0, 0, 1), minvec);

    p[3] = plane(vec3(-1, 0, 0), maxvec);
    p[4] = plane(vec3(0, -1, 0), maxvec);
    p[5] = plane(vec3(0, 0, -1), maxvec);

    bool res2 = true;
    for( i32 c = 0; c < 6; ++c )
    {
      bool r = false;

      for( i32 d = 0; d < 8; ++d )
      {
        r = r || !(p[c].distance(a->points[d]) < 0);
      }

      if( !r )
      {
        res2 = false;
        break;
      }
    }

    return res || res2;*/
    /**/

    /**
    static vector<vec3> v;
    v.clear();
    v = b->get_vertices();

    bool res = true;
    for( i32 c = 0; c < 6; ++c )
    {
      bool r = false;

      for( i32 d = 0; d < 8; ++d )
      {
        r = r || !(static_cast<plane*>(a->planes[c])->distance(v[d]) < 0);
      }

      if( !r )
      {
        res = false;
        break;
      }
    }

    vec3 minvec = b->min;
    vec3 maxvec = b->max;

    plane p[6];
    p[0] = plane(vec3(1, 0, 0), minvec);
    p[1] = plane(vec3(0, 1, 0), minvec);
    p[2] = plane(vec3(0, 0, 1), minvec);

    p[3] = plane(vec3(-1, 0, 0), maxvec);
    p[4] = plane(vec3(0, -1, 0), maxvec);
    p[5] = plane(vec3(0, 0, -1), maxvec);

    bool res2 = true;
    for( i32 c = 0; c < 6; ++c )
    {
      bool r = false;

      for( i32 d = 0; d < 8; ++d )
      {
        r = r || !(p[c].distance(a->points[d]) < 0);
      }

      if( !r )
      {
        res2 = false;
        break;
      }
    }

    bool ret = res || res2;

    if( ret )
      return true;
    else
      return false;
    /**/
  }

  static bool intersect_af( shape* aa, shape* bb )
  {
    return intersect_fa(bb, aa);
  }

  //is a inside b?
  static bool is_inside_sa( shape* aa, shape* bb )
  {
    auto a = static_cast<sphere*>(aa);
    auto b = static_cast<aabb*>(bb);

    mm::vec3 spheremax = a->center + a->radius;
    mm::vec3 spheremin = a->center - a->radius;

    if( mm::lessThanEqual(spheremax, b->max) && mm::greaterThanEqual(spheremin, b->min) )
      return true;

    return false;
  }

  //is a inside b?
  static bool is_inside_as( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<sphere*>(bb);

    mm::vec3 minvec = a->min - b->center;
    mm::vec3 maxvec = a->max - b->center;
    f32 sqrmaxlength = mm::dot(maxvec, maxvec);
    f32 sqrminlength = mm::dot(minvec, minvec);
    f32 sqrradius = b->radius * b->radius;

    if( sqrmaxlength <= sqrradius && sqrminlength <= sqrradius )
      return true;

    return false;
  }

  //is a inside b?
  static bool is_inside_aa( shape* aa, shape* bb )
  {
    auto a = static_cast<aabb*>(aa);
    auto b = static_cast<aabb*>(bb);

    if( mm::greaterThanEqual( a->min, b->min ) && mm::lessThanEqual( a->max, b->max ) )
      return true;

    return false;
  }

  //is a inside b?
  static bool is_inside_ss( shape* aa, shape* bb )
  {
    auto a = static_cast<sphere*>(aa);
    auto b = static_cast<sphere*>(bb);

    mm::vec3 spheredist = b->center - a->center;

    if( mm::dot(spheredist, spheredist) <= b->radius * b->radius )
      return true;

    return false;
  }
}

void shape::set_up_intersection()
{
  //order doesnt matter
  _is_on_right_side.add<sphere, plane>(inner::is_on_right_side_sp);
  _is_on_right_side.add<plane, sphere>(inner::is_on_right_side_ps);
  _is_on_right_side.add<aabb, plane>(inner::is_on_right_side_ap);
  _is_on_right_side.add<plane, aabb>(inner::is_on_right_side_pa);

  _intersects.add<aabb, aabb>(inner::intersect_aa);
  _intersects.add<aabb, plane>(inner::intersect_ap);
  _intersects.add<plane, aabb>(inner::intersect_pa);
  _intersects.add<aabb, sphere>(inner::intersect_as);
  _intersects.add<sphere, aabb>(inner::intersect_sa);
  _intersects.add<plane, plane>(inner::intersect_pp);
  _intersects.add<sphere, plane>(inner::intersect_sp);
  _intersects.add<plane, sphere>(inner::intersect_ps);
  _intersects.add<sphere, sphere>(inner::intersect_ss);
  _intersects.add<frustum, sphere>(inner::intersect_fs);
  _intersects.add<sphere, frustum>(inner::intersect_sf);
  _intersects.add<frustum, aabb>(inner::intersect_fa);
  _intersects.add<aabb, frustum>(inner::intersect_af);

  //order matters
  _is_inside.add<aabb, aabb>(inner::is_inside_aa);
  _is_inside.add<aabb, sphere>(inner::is_inside_as);
  _is_inside.add<sphere, aabb>(inner::is_inside_sa);
  _is_inside.add<sphere, sphere>(inner::is_inside_ss);

  //usage
  //is_on_the_right_side.go(new aabb(), new sphere());

  is_setup = true;
}

#endif

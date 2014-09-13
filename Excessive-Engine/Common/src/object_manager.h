#ifndef object_manager_h
#define object_manager_h

#include <limits.h>
#include <vector>
#include <assert.h>
#include "basic_types.h"

namespace om
{

  /**/
  //64 bit IDs
  #define INDEX_MASK (u64(0xffffffff))
  #define INNER_MASK (u32(0xffffffff))
  #define NEW_OBJECT_ID_ADD (u64(0x100000000))
  typedef u64 id_type;
  typedef u32 inner_id_type;
  /**/

  /**
  //32 bit IDs
  #define INDEX_MASK 0xffff
  #define INNER_MASK USHRT_MAX
  #define NEW_OBJECT_ID_ADD 0x10000
  typedef u32 id_type;
  typedef u16 inner_id_type;
  /**/

  //32 + 16 + 16 bits
  //OR
  //64 + 32 + 32 bits
  struct index
  {
    id_type id; //unique object identifier
    inner_id_type idx; //index to the object buffer
    inner_id_type next; //next free index
    index( id_type i = INDEX_MASK, inner_id_type n = INDEX_MASK, inner_id_type ix = INDEX_MASK ) :
      id( i ), idx( ix ), next( n ) {}
  };

  template< class t >
  class object_manager
  {
    private:
      typedef std::pair< id_type, t > stored_type;

      //this stores the objects themselves
      //first is the id (mask included)
      //second is the data
      std::vector< stored_type > objects;

      //this stores the occupied AND freed indices in the list
      //if an item is freed it's index remains here,
      //so that we can indicate if someone tries to access it.
      //so indices are NOT recycled
      std::vector< index > indices;

      //stores the index where to delete from
      inner_id_type freelist_enqueue;

      //stores the index where to insert to
      inner_id_type freelist_dequeue;
    protected:
    public:
      typedef typename std::vector< stored_type >::iterator iter;

      bool has( id_type id )
      {
        index& in = indices[id & INDEX_MASK];
        return in.id == id && in.idx != INNER_MASK;
      }

      t& lookup( id_type id )
      {
        ASSERT( has( id ) );
        return objects[indices[id & INDEX_MASK].idx].second;
      }

      id_type add( const t& d )
      {
        //no indices stored, or no space left for more indices
        if( freelist_dequeue == INNER_MASK || freelist_dequeue == indices.size() )
        {
          freelist_dequeue = om::id_type( indices.size() );
          freelist_dequeue = om::id_type( indices.size() );
          indices.push_back( index( indices.size(), indices.size() + 1 ) );
        }

        index& in = indices[freelist_dequeue];
        freelist_dequeue = in.next;
        in.id += NEW_OBJECT_ID_ADD;
        in.idx = objects.size();
        objects.push_back( stored_type( 0, d ) );
        stored_type& o = objects[in.idx];
        o.first = in.id;
        return in.id;
      }

      void remove( id_type id )
      {
        index& in = indices[id & INDEX_MASK];

        stored_type& o = objects[in.idx];
        o = objects[objects.size() - 1];
        indices[o.first & INDEX_MASK].idx = in.idx;
        objects.resize( objects.size() - 1 );
        in.idx = INNER_MASK;

        //no indices stored, or no space left for more indices
        if( freelist_enqueue == INNER_MASK || freelist_enqueue == indices.size() )
        {
          freelist_enqueue = indices.size() - 1;
        }

        indices[freelist_enqueue].next = id & INDEX_MASK;
        freelist_enqueue = id & INDEX_MASK;
      }

      std::vector< stored_type >& get_objects()
      {
        return objects;
      }

      iter begin()
      {
        return objects.begin();
      }

      iter end()
      {
        return objects.end();
      }

      object_manager()
      {
        freelist_enqueue = INNER_MASK;
        freelist_dequeue = INNER_MASK;
      }
  };

}

#endif

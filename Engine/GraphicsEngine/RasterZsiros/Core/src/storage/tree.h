////////////////////////////////////////////////////////////////////////////////
//	file: tree_adv.h
//	content: a generic tree class, it stores elements in hierarchy
//			supports iterators
//
////////////////////////////////////////////////////////////////////////////////
//	documentation:
//		container structure:
//			hierarchical arrays of nodes, connected
//			the first array is connected to an inaccessible root (makes implementation a lot easier)
//		functions:
//			empty: return whether the container is empty
//			
//			begin(void): iterator to the first element of the first array (the array below the root)
//			begin(const iterator&): iterator to the first element in the array of arg1
//			end(void): iterator to the element after the last element (like standard stl)
//			end(const iterator&): iterator to the element after the last element of the array of arg1
//				-array of arg1 means the array that contains the neighbours of arg1
//
//			insert: inserts an element to the first place in the array below root, begin() returns this element
//			insert_after: inserts an element after the iterator, in the same array.
//				Iterator might be end(), but in this case 'insert' is used, since end() has no array.
//			insert_before: inserts and element before the iterator, in the same array. 
//				Iterator might be end(), but in this case 'insert' is used, since end() has no array.
//			insert_child: insert a child to the first place among the element's current children.
//				Inserts the child even if there are no any other children yet.
//			erase: erases the element pointed to, and all its children, grandchildren etc., all its belongings.
//			
//			clear: clears every element, reseting the container
//		iterators:
//			traversal:
//				++, --, +(go up), -(go down)
//				++/-- jumps to unrelated places when the current array is finished
//				+/- returns end() if it can't go higher or deeper, use can_go_up/can_go_down to avoid this
//			notes:
//				const_iterator: will be implemented, if i'm not that lazy
//		other:
//			-Construction possible from another container with different type, if, and only if
//			 the data type of the container is constructible from the other container's data type.
//			 In this case the inner structure and hierarchy is preserved.
//
////////////////////////////////////////////////////////////////////////////////



#pragma once

#include <cassert>
#include <memory>
#include <iostream>


namespace tree_ns_priv {

struct node_base {

};

};



////////////////////////////////////////////////////////////////////////////////
// template class: tree<T>
//
////////////////////////////////////////////////////////////////////////////////
template <class T>
class tree {
		// friends
		template <class U> friend class tree;
		// pre defines
		struct node;
	public:
		// typedefs
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;


		// iterator class
		class iterator {
			public:
				friend int main();
				friend class tree;

				// ctor & dtor
				iterator();	//........................................................ OK
				~iterator(); //....................................................... OK
				// copy
				iterator(const iterator&); //......................................... OK
				iterator& operator=(const iterator&); //.............................. OK
				// traversal
				iterator& operator++(); // next element on the level //............... seems OK
				iterator  operator++(int) {iterator r=*this; ++*this; return r;}
				iterator& operator--(); // previous element on the level //........... seems OK
				iterator  operator--(int) {iterator r=*this; --*this; return r;}
				iterator& up(); // go one level upper in hierarchy //................. OK
				iterator& operator+() {return this->up();}
				bool can_go_up();
				iterator& down(); // go one level deeper //........................... OK
				iterator& operator-() {return this->down();}
				bool can_go_down();
				iterator begin() {return _pContainer->begin(*this);}
				iterator end() {return _pContainer->end(*this);}
				// dereference
				T& operator*(); //.................................................... OK
				T* operator->();
				// compare
				bool operator==(const iterator& other) const; //...................... OK
				bool operator!=(const iterator& other) const {return !((*this)==other);}
				// misc
				tree& get_tree() {return *_pContainer;}
			private:
				bool _is_last(); // returns whether it's th last in its array //...... OK
				bool _is_first(); //.................................................. OK
				bool _lastMove; // hint about where did we get to this iterator: by going down&side or going up
				#define FROM_BELOW true
				#define FROM_LEFT_OR_UP false
				node* _ptr; // the node pointed to
				tree* _pContainer;
		};
	private:
		// node struct
		struct node : public tree_ns_priv::node_base {
				typedef tree tree_dummy_t;

				friend class tree;
				friend class iterator;
				// node's data
				T data;
				// functions
				node& parent() {return *pParent;}
				size_t num_children() {return nChildren;}
				size_t add_child(size_t idx);
				size_t add_child(size_t idx, const T& data);
				void remove_child(size_t idx);
				// constructor & destructor
				node();
				node(const T& data);
				~node(); // deletes all child nodes
				// operator= and copy constructor
				node(const node& other);
				template <class O> node(const O& other);
				node& operator=(const node& other);
				template <class O> node& operator=(const O& other);
				template <class O> node& copy_structure(const O& other);

				node* pParent;
				node** ppChildren;
				size_t nChildren;
				size_t index; // this is the index of its position in an array
		};

		// the root node
		node* pRoot;

	public:
		// constructor, destructor
		tree();
		~tree();
		// copy
		tree(const tree& other);
		template <class O> tree(const tree<O>& other);
		tree& operator=(const tree& other);
		template <class O> tree& operator=(const tree<O>& other);

		// capacity
		bool empty() const {return (pRoot==nullptr);} // test whether it's empty

		// iterators //............................................................... OK
		iterator begin(); // normal begin iterator
		iterator end(); // normal end iterator
		iterator begin(const iterator& it); // iterator to the first element of the subarray containing 'it'
		iterator end(const iterator& it); // iterator to the element past the subarray containing 'it'

		// modifiers //............................................................... OK
		iterator insert(const value_type& data);
		iterator insert_before(const iterator&, const value_type&);
		iterator insert_after(const iterator&, const value_type&);
		iterator insert_child(const iterator&, const value_type&);
		void erase(iterator& it);

		// misc
		void clear();	
		template <class U>
		tree& copy_structure(tree<U>& from);
};











////////////////////////////////////////////////////////////////////////////////
// tree iterator



// ctor & dtor /////////////////////////
template <class T>
tree<T>::iterator::iterator() {
	_ptr = nullptr;
	_lastMove = FROM_LEFT_OR_UP;
	_pContainer = nullptr;
	return;
}
template <class T>
tree<T>::iterator::~iterator() {
	return;
}



// copy ////////////////////////////////
template <class T>
tree<T>::iterator::iterator(const iterator& other) {
	_ptr = other._ptr;
	_lastMove = other._lastMove;
	_pContainer = other._pContainer;
}
template <class T>
typename tree<T>::iterator& tree<T>::iterator::operator=(const iterator& other) {
	_ptr = other._ptr;
	_lastMove = other._lastMove;
	_pContainer = other._pContainer;
	return *this;
}



// traversal ///////////////////////////

// true if it's the last in the array
template <class T>
bool tree<T>::iterator::_is_last() {
	if (!_ptr->pParent)
		return true;
	size_t idx=_ptr->index; // (size_t(_ptr)-size_t(_ptr->pParent->ppChildren))/sizeof(void*);
	if (idx+1==_ptr->pParent->nChildren)
		return true;
	else
		return false;
}
template <class T>
bool tree<T>::iterator::_is_first() {
	if (!_ptr)
		return false;
	else if (!_ptr->pParent)
		return true;
	else
		return (_ptr->pParent->ppChildren[0] == _ptr);
}



// next element on the level
template <class T>
typename tree<T>::iterator& tree<T>::iterator::operator++() { 
	// if it's the root, iteration ended
	if (!_ptr || !_ptr->pParent) {
		_ptr = nullptr;
		return *this;
	}

	node** ppNodes = _ptr->pParent->ppChildren; // the table of pointers to neighbours	
	size_t idx=_ptr->index; // (size_t(_ptr)-size_t(ppNodes))/sizeof(void*); // index of this node in the array 

	
	if (_lastMove == FROM_BELOW) {
		if (_is_last()) {
			// go up
			_ptr = _ptr->pParent;
			_lastMove = FROM_BELOW;
			return ++(*this); // restart procedure
		}
		else {
			// go to neighbours' children
			node** ppNeighbours = _ptr->pParent->ppChildren;
			size_t nNeighbours = _ptr->pParent->nChildren;
			for (size_t i=idx+1; i<nNeighbours; i++) {
				if (ppNeighbours[i]->nChildren>0) {
					_ptr = ppNeighbours[i]->ppChildren[0];
					_lastMove = FROM_LEFT_OR_UP;
					return *this;
				}
			}
			// no more children among fathers: go up
			_ptr = _ptr->pParent;
			_lastMove = FROM_BELOW;
			return ++(*this); // restart procedure
		}
	}
	else {
		if (_is_last()) {
			// look for children in the current array
			for (size_t i=0; i<=idx; i++) {
				if (ppNodes[i]->nChildren>0) {
					_ptr = ppNodes[i]->ppChildren[0];
					_lastMove = FROM_LEFT_OR_UP;
					return *this;
				}
			}
			// if children not found, set flag to 'FROM_BELOW', and restart
			_lastMove = FROM_BELOW;
			return ++(*this);
		}
		else {
			_ptr = ppNodes[idx+1];
			_lastMove = FROM_LEFT_OR_UP;
			return *this;
		}
	}
}

// previous element on the level
template <class T>
typename tree<T>::iterator& tree<T>::iterator::operator--() { 	
	// if it's the begin() iterator iteration ended
	if (_is_first() && _ptr->pParent->pParent==nullptr) {
		_ptr = nullptr;
		return *this;
	}
	// if it's the end() iterator, find the last one: the same operation as in a below chunk of code
	if (_ptr==nullptr) {
			if (!_pContainer || !_pContainer->pRoot) {
				return *this;
			};
			// go to the deepest point of this branch while chosing the rightmost sub-branch
			node** thisArray;
			node* nearestToLeft = _pContainer->pRoot->ppChildren[_pContainer->pRoot->nChildren-1];
			do {
				_ptr = nearestToLeft->ppChildren[nearestToLeft->nChildren-1];
				thisArray = _ptr->pParent->ppChildren;
				nearestToLeft = nullptr;
				for (ptrdiff_t i=_ptr->pParent->nChildren-1; i>=0; i--) {
					if (thisArray[i]->nChildren>0) {
						nearestToLeft = thisArray[i];
						break;
					}
				}
			}
			while (nearestToLeft);
			// _ptr now points to the deespest and rightmost element
			_lastMove = FROM_LEFT_OR_UP;
			return *this;
	}

	node** ppNodes = _ptr->pParent->ppChildren; // the table of pointers to neighbours	
	size_t idx=_ptr->index; //(size_t(_ptr)-size_t(ppNodes))/sizeof(void*); // index of this node in the array 

	
	if (_lastMove == FROM_BELOW) {
		// we must've come here because it's the first item of an array (being begin() handled before)

		// check if this would come after finishing the parent array
		// this can be done by checking parent array's elements on the left of parent
		node** ppParentArray = _ptr->pParent->pParent->ppChildren;
		node* pParent = _ptr->pParent;
		node* nearestToLeft = nullptr;
		// this iterates over elements in the parent array, which are on the left of parent
		for (ptrdiff_t i=pParent->index-1 /*(size_t(pParent)-size_t(ppParentArray))/sizeof(void*)-1*/; i>=0; i--) {
			if (ppParentArray[i]->nChildren>0) {
				nearestToLeft = ppParentArray[i];
				break;
			}
		}
		// nullptr indicates that this would come after finishing the parent array
		if (nearestToLeft == nullptr) {
			// set the iterator to the last element of the parent array
			_ptr = ppParentArray[pParent->pParent->nChildren-1];
			_lastMove = FROM_LEFT_OR_UP;
			return *this;
		}
		// this is not the one that would come after the last of parent array
		else  {
			// go to the deepest point of this branch while chosing the rightmost sub-branch
			node** thisArray;
			do {
				_ptr = nearestToLeft->ppChildren[nearestToLeft->nChildren-1];
				thisArray = _ptr->pParent->ppChildren;
				nearestToLeft = nullptr;
				for (ptrdiff_t i=_ptr->pParent->nChildren-1; i>=0; i--) {
					if (thisArray[i]->nChildren>0) {
						nearestToLeft = thisArray[i];
						break;
					}
				}
			}
			while (nearestToLeft);
			// _ptr now points to the deespest and rightmost element
			_lastMove = FROM_LEFT_OR_UP;
			return *this;
		}
	}
	else {
		// if it's the first, go back upwards, and restart procedure
		if (_is_first()) {
			_lastMove = FROM_BELOW;
			return --(*this);
		}
		// if it's not the first, we must've come from the left neighbour: let's go right
		else {
			_ptr = ppNodes[idx-1];
			_lastMove = FROM_LEFT_OR_UP; // way of travel unchanged
			return *this;
		}
	}
}

// go one level upper in hierarchy
template <class T>
typename tree<T>::iterator& tree<T>::iterator::up() {
	if (_ptr!=nullptr && _ptr->pParent->pParent!=nullptr)
		_ptr = _ptr->pParent;
	else
		_ptr = nullptr;
	return *this;
}

// go one level deeper
template <class T>
typename tree<T>::iterator& tree<T>::iterator::down() { 
	if (_ptr != nullptr && _ptr->nChildren>0)
		_ptr = _ptr->ppChildren[0];
	else 
		_ptr = nullptr;
	return *this;
}

template <class T>
bool tree<T>::iterator::can_go_up() {
	return (_ptr!=nullptr && _ptr->pParent->pParent != nullptr);
}
template <class T>
bool tree<T>::iterator::can_go_down() {
	return (_ptr!=nullptr && _ptr->nChildren>0);
}



// dereference /////////////////////////
template <class T>
T& tree<T>::iterator::operator*() {
	assert(_ptr);
	return _ptr->data;
}
template <class T>
T* tree<T>::iterator::operator->() {
	return &_ptr->data;
}



// compare /////////////////////////////
template <class T>
bool tree<T>::iterator::operator==(const iterator& other) const {
	return (_ptr==other._ptr);
}








////////////////////////////////////////////////////////////////////////////////
// tree node

// add child ///////////////////////////
template <class T>
size_t tree<T>::node::add_child(size_t idx) {
	return add_child(idx, T());
}
template <class T>
size_t tree<T>::node::add_child(size_t idx, const T& data) {
	// set idx to the last pos if it's out of range
	if (idx>nChildren)
		idx = nChildren;

	// allocate the new node
	node* pNewChild = new node(data);
	assert(pNewChild);
	pNewChild->pParent = this;
	pNewChild->index = idx;
	// allocate the new children node* table
	node** ppNewChildren = nullptr;
	ppNewChildren = new node*[nChildren+1u];
	assert(ppNewChildren);

	// copy old pointers and insert new child
	for (size_t i=0; i<idx; i++) {
		ppNewChildren[i] = ppChildren[i];
	}
	for (size_t i=idx; i<nChildren; i++) {
		ppChildren[i]->index++;
		ppNewChildren[i+1] = ppChildren[i];
	}

	ppNewChildren[idx] = pNewChild;

	// post-job
	nChildren++;
	delete[] ppChildren;
	ppChildren = ppNewChildren;
	return idx;
}

// remove child ////////////////////////
template <class T>
void tree<T>::node::remove_child(size_t idx) {
	if (idx>=nChildren)
		return;

	node** ppNewChildren = nullptr;

	if (nChildren>1) {		
		ppNewChildren = new node*[nChildren-1];

		assert(ppNewChildren);

		// copy old stuff
		for (size_t i=0; i<idx; i++) {
			ppNewChildren[i] = ppChildren[i];
		}
		for (size_t i=idx+1; i<nChildren; i++) {
			ppChildren[i]->index--; // decrease index
			ppNewChildren[i-1] = ppChildren[i];
		}
	}

	nChildren--; // reduce number of children
	
	// parent is set to null in destructor of the children, preventing double deletion
	if (ppChildren[idx]->pParent) {
		ppChildren[idx]->pParent=nullptr;
		delete ppChildren[idx]; // delete this node
	}

	delete[] ppChildren; // delete old children table
	ppChildren = ppNewChildren; // copy new
}

// ctors ///////////////////////////////
template <class T>
tree<T>::node::node() {
	nChildren = 0;
	ppChildren = nullptr;
	pParent = nullptr;
};

template <class T>
tree<T>::node::node(const T& data) {
	nChildren = 0;
	this->data = data;
	ppChildren = nullptr;
	pParent = nullptr;
};

// destructor //////////////////////////
template <class T>
tree<T>::node::~node() {
	// delete all child nodes -OR- collapse child nodes with *pParent's child nodes
	for (size_t i=0; i<nChildren; i++) {
		ppChildren[i]->pParent = nullptr; // this node is the parent, and this is being deleted, no need to remove children one-by-one
		delete ppChildren[i];
	}
	delete[] ppChildren;
	// parent is set to null in remove_child(), preventing double removal
	if (pParent) {
		node* pParentDummy = pParent;
		pParent = nullptr;
		pParentDummy->remove_child(index);
	}
};


// node cloning ////////////////////////

// copy contruct
template <class T>
tree<T>::node::node(const node& other)
	:	data(other.data),
		index(other.index),
		nChildren(other.nChildren),
		pParent(nullptr)
{
	// copy children
	if (nChildren>0) {
		ppChildren = new node*[nChildren];
		for (size_t i=0; i<nChildren; i++) {
			ppChildren[i] = new node(*other.ppChildren[i]); // copy-contruct children from other's children
			ppChildren[i]->pParent = this; // set the parent of copied children
		}
	}
	else {
		ppChildren = nullptr;
	}
}
// copy
template <class T>
typename tree<T>::node& tree<T>::node::operator=(const node& other) {
	// note: the same as copy ctr, but here, parent stays the same for this node
	//		 this node is still set as parent for children

	// copy data members
	data = other.data;
	index = other.index;
	nChildren = other.nChildren;

	// copy children
	if (nChildren>0) {
		ppChildren = new node*[nChildren];
		for (size_t i=0; i<nChildren; i++) {
			ppChildren[i] = new node(*other.ppChildren[i]); // copy-contruct children from other's children
			ppChildren[i]->pParent = this; // set the parent of copied children
		}
	}
	else {
		ppChildren = nullptr;
	}
	return *this;
}

// template copy contruct
template <class T>
template <class O>
tree<T>::node::node(const O& other) 
	:	data(other.data),
		index(other.index),
		nChildren(other.nChildren),
		pParent(nullptr)
{
	static_assert(std::is_base_of<tree_ns_priv::node_base, typename std::decay<O>::type>::value, "error: not a tree node");

	// copy children
	if (nChildren>0) {
		ppChildren = new node*[nChildren];
		for (size_t i=0; i<nChildren; i++) {
			ppChildren[i] = new node(*other.ppChildren[i]); // copy-contruct children from other's children
			ppChildren[i]->pParent = this; // set the parent of copied children
		}
	}
	else {
		ppChildren = nullptr;
	}
}
// template copy
template <class T>
template <class O>
typename tree<T>::node& tree<T>::node::operator=(const O& other) {
	// fail if <O> not a tree node
	static_assert(std::is_base_of<tree_ns_priv::node_base, typename std::decay<O>::type>::value, "error: not a tree node");

	// note: the same as copy ctor, but here, parent stays the same for this node
	//		 this node is still set as parent for children

	// copy data members
	data = other.data;
	index = other.index;
	nChildren = other.nChildren;

	// copy children
	if (nChildren>0) {
		ppChildren = new node*[nChildren];
		for (size_t i=0; i<nChildren; i++) {
			ppChildren[i] = new node(*other.ppChildren[i]); // copy-contruct children from other's children
			ppChildren[i]->pParent = this; // set the parent of copied children
		}
	}
	else {
		ppChildren = nullptr;
	}
	return *this;
}
// template copy only subnodes, no data copy
template <class T>
template <class O> 
typename tree<T>::node& tree<T>::node::copy_structure(const O& other) {
	// assert on failure
	static_assert(std::is_base_of<tree_ns_priv::node_base, typename std::decay<O>::type>::value, "error: not a tree node");

	// copy data members
	index = other.index;
	nChildren = other.nChildren;

	// copy children
	if (nChildren>0) {
		ppChildren = new node*[nChildren];
		for (size_t i=0; i<nChildren; i++) {
			ppChildren[i] = new node();
			ppChildren[i]->copy_structure(*other.ppChildren[i]);
			ppChildren[i]->pParent		= this; // set the parent of copied children

			node::data; node::index; node::nChildren; node::pParent; node::ppChildren;
		}
	}
	else {
		ppChildren = nullptr;
	}
	return *this;
}










////////////////////////////////////////////////////////////////////////////////
// tree class

// constructor, destructor, copy /////////////
template <class T>
tree<T>::tree() {
	pRoot = nullptr;
}
template <class T>
tree<T>::~tree() {
	if (pRoot != nullptr)
		delete pRoot;
	int a=5;
}

// construct from other
template <class T>
tree<T>::tree(const tree& other) {
	if (other.pRoot)
		pRoot = new node(*other.pRoot);
	else
		pRoot = nullptr;
}
// construct from other w/ different type
template <class T>
template <class O> 
tree<T>::tree(const tree<O>& other) {
	if (other.pRoot) {
		pRoot = new node(*other.pRoot);
	}
	else {
		pRoot = nullptr;
	}
}

// copy from other
template <class T>
tree<T>& tree<T>::operator=(const tree& other) {
	if (pRoot != nullptr)
		delete pRoot;
	if (other.pRoot)
		pRoot = new node(*other.pRoot);
	else
		pRoot = nullptr;
	return *this;
}
// copy from other w/ different type
template <class T>
template <class O>
tree<T>& tree<T>::operator=(const tree<O>& other) {
	if (pRoot != nullptr)
		delete pRoot;
	if (other.pRoot)
		pRoot = new node(*other.pRoot);
	else
		pRoot = nullptr;
	return *this;
}



// iterators ///////////////////////////
template <class T>
typename tree<T>::iterator tree<T>::begin() {
	if (pRoot) {
		iterator it;
		it._pContainer = this;
		it._ptr = pRoot->ppChildren[0];
		it._lastMove = FROM_LEFT_OR_UP;
		return it;
	}
	else {
		return end();
	}
}
template <class T>
typename tree<T>::iterator tree<T>::end() {
	iterator it;
	it._pContainer = this;
	it._ptr = nullptr;
	it._lastMove = FROM_LEFT_OR_UP;
	return it;
}
template <class T>
typename tree<T>::iterator tree<T>::begin(const iterator& it) {
	assert(it._pContainer==this);
	if (!it._ptr)
		return end();
	iterator rv;
	rv._pContainer = this;
	node* firstNode = it._ptr->pParent->ppChildren[0];
	rv._ptr = firstNode;
	rv._lastMove = FROM_LEFT_OR_UP;
	return rv;
}
template <class T>
typename tree<T>::iterator tree<T>::end(const iterator& it) {
	assert(it._pContainer==this);
	if (!it._ptr)
		return end();
	iterator rv;
	rv._pContainer = this;
	node* lastNode = it._ptr->pParent->ppChildren[it._ptr->pParent->nChildren-1];
	rv._ptr = lastNode;
	rv._lastMove = FROM_LEFT_OR_UP;
	return ++rv;
}



// modifiers ///////////////////////////

// inserts an element just below root, it'll actually be the begin() item
template <class T>
typename tree<T>::iterator tree<T>::insert(const value_type& data) {
	if (pRoot == nullptr) {
		pRoot = new node(data);
	}
	iterator rv;
	rv._lastMove=FROM_LEFT_OR_UP;
	rv._pContainer=this;
	rv._ptr = pRoot->ppChildren[pRoot->add_child(0, data)];
	return rv;
}

// note that the below two function insert elements into the same array as 'it' is in
template <class T>
typename tree<T>::iterator tree<T>::insert_before(const iterator& it, const value_type& data) {
	// assert if the iterator doesn't belong to this tree
	assert(it._pContainer==this);
	// if it's the end iterator
	if (it._ptr == nullptr) {
		return insert(data);
	}
	// the iterator returned
	iterator rv;
	rv._pContainer = this;
	// simply add the new item in place of this, and slide the others
	size_t idxOfThis = it._ptr->index;
	size_t idx_rv = it._ptr->pParent->add_child(idxOfThis, data);
	rv._ptr = it._ptr->pParent->ppChildren[idx_rv];

	return rv;
}
template <class T>
typename tree<T>::iterator tree<T>::insert_after(const iterator& it, const value_type& data) {
	// assert if the iterator doesn't belong to this tree
	assert(it._pContainer==this);
	// if it's the end iterator
	if (it._ptr == nullptr) {
		return insert(data);
	}
	// the iterator returned
	iterator rv;
	rv._pContainer = this;
	// add the new item in place of the next
	size_t idxOfThis = it._ptr->index;
	size_t idx_rv = it._ptr->pParent->add_child(idxOfThis+1, data);
	rv._ptr = it._ptr->pParent->ppChildren[idx_rv];

	return rv;
}

// insert a child, may used if there are already children, but it'll be inserted to index=0
template <class T>
typename tree<T>::iterator tree<T>::insert_child(const iterator& it, const value_type& data) {
	// assert if the iterator doesn't belong to this tree
	assert(it._pContainer==this);
	// if it's the end iterator
	if (it._ptr == nullptr) {
		return insert(data);
	}
	// the iterator returned
	iterator rv;
	rv._pContainer = this;
	rv._ptr = it._ptr->ppChildren[it._ptr->add_child(0, data)];
	return rv;
}

// erases the whole branch under 'it', BE AWARE!
template <class T>
void tree<T>::erase(iterator& it) {
	delete it._ptr;
	if (pRoot->nChildren==0) {
		delete pRoot;
		pRoot = nullptr;
	}
}

// clear all the elements, even pRoot
template <class T>
void tree<T>::clear() {
	if (pRoot) {
		delete pRoot;
		pRoot = nullptr;
	}
}

// copy the structure of a tree, with invoking default contructors, thus no element copy
template <class T>
template <class U>
typename tree<T>& tree<T>::copy_structure(tree<U>& from) {
	if (pRoot != nullptr)
		delete pRoot;

	if (from.pRoot) {
		pRoot = new node();
		pRoot->copy_structure(*from.pRoot);
	}
	else {
		pRoot = nullptr;
	}
	return *this;	
}
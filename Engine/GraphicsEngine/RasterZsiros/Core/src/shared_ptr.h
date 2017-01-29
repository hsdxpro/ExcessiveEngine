////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	This is a reference counting pointer, that calls a deleter when no one
//	points to the object any longer.
//	It's totally like STL, EXCEPT that it'll always know the reference count, no
//	matter where and how you assign it a value.
////////////////////////////////////////////////////////////////////////////////


#include <unordered_map>
#include <functional>
#include <cassert>
#include <ostream>


#pragma once


template <class T>
class zs_shared_ptr {
public:
	// construct
	zs_shared_ptr();
	zs_shared_ptr(nullptr_t);
	template <class Y>
	zs_shared_ptr(Y* ptr);
	template <class Y, class Deleter>
	zs_shared_ptr(Y* ptr, Deleter d);

	zs_shared_ptr(const zs_shared_ptr& r);
	zs_shared_ptr(zs_shared_ptr&& r);
	// destruct
	~zs_shared_ptr();

	// assignment
	zs_shared_ptr& operator=(const zs_shared_ptr& r);
	zs_shared_ptr& operator=(zs_shared_ptr&& r);

	// modifiers
	void swap(zs_shared_ptr& other);

	// observers
	T* get() const;
	T& operator*() const;
	T* operator->() const;
	size_t use_count() const;
	bool unique() const;
	operator bool() const;
private:
	void register_me();
	size_t unregister_me();
	void destroy_me();
	
	static std::unordered_map<T*, size_t> counts;
	std::function<void(T*)> deleter;
	T* ptr;
};

// kibaszott static, kibaszott linker error
template <class T>
std::unordered_map<T*, size_t> zs_shared_ptr<T>::counts;


// register new pointer
template <class T>
void zs_shared_ptr<T>::register_me() {
	if (ptr != nullptr) {
		auto it = counts.insert(std::pair<T*, size_t>(ptr, 0));
		it.first->second++;
	}
}
template <class T>
size_t zs_shared_ptr<T>::unregister_me() {
	if (ptr != nullptr) {
		auto it = counts.find(ptr);
		assert(it != counts.end());
		it->second--;
		if (it->second != 0) {
			return it->second;
		}
		else {
			counts.erase(it);
			return 0;
		}
	}
	return 0;
}
// destroy pointed object
template <class T>
void zs_shared_ptr<T>::destroy_me() {
	if (ptr != nullptr) {
		if (deleter)
			deleter(ptr);
		else
			delete ptr;
	}
}



// constructors
template <class T>
zs_shared_ptr<T>::zs_shared_ptr() : ptr(nullptr) {
}

template <class T>
zs_shared_ptr<T>::zs_shared_ptr(nullptr_t) : ptr(nullptr) {
}

template <class T>
template <class Y>
zs_shared_ptr<T>::zs_shared_ptr(Y* ptr) : ptr(ptr) {
	register_me();
}

template <class T>
template <class Y, class Deleter>
zs_shared_ptr<T>::zs_shared_ptr(Y* ptr, Deleter d) : ptr(ptr), deleter(d) {
	register_me();
}


template <class T>
zs_shared_ptr<T>::zs_shared_ptr(const zs_shared_ptr& r) : ptr(r.ptr), deleter(r.deleter) {
	register_me();
}
template <class T>
zs_shared_ptr<T>::zs_shared_ptr(zs_shared_ptr&& r) : ptr(r.ptr), deleter(r.deleter) {
	other.ptr = nullptr;
}

// destruct
template <class T>
zs_shared_ptr<T>::~zs_shared_ptr() {
	if (unregister_me() == 0)
		destroy_me();
}

// assign
template <class T>
zs_shared_ptr<T>& zs_shared_ptr<T>::operator=(const zs_shared_ptr& r) {
	if (unregister_me() == 0)
		destroy_me();
	ptr = r.ptr;
	if (r.deleter)
		deleter = r.deleter;
	register_me();
	return *this;
}

template <class T>
zs_shared_ptr<T>& zs_shared_ptr<T>::operator=(zs_shared_ptr&& r) {
	if (unregister_me() == 0)
		destroy_me();
	ptr = r.ptr;
	r.ptr = nullptr;
	if (r.deleter)
		deleter = r.deleter;
	return *this;
}

// observers
template <class T>
T* zs_shared_ptr<T>::get() const {
	return ptr;
}

template <class T>
T& zs_shared_ptr<T>::operator*() const {
	return *ptr;
}

template <class T>
T* zs_shared_ptr<T>::operator->() const {
	return ptr;
}

template <class T>
size_t zs_shared_ptr<T>::use_count() const {
	auto it = counts.find(ptr);
	return it->second;
}

template <class T>
bool zs_shared_ptr<T>::unique() const {
	use_count() == 1;
}

template <class T>
zs_shared_ptr<T>::operator bool() const {
	return ptr != nullptr;
}


// modifiers
template <class T>
void zs_shared_ptr<T>::swap(zs_shared_ptr& r) {
	auto f_tmp = deleter;
	deleter = r.deleter;
	r.deleter = f_tmp;

	auto ptr_tmp = ptr;
	ptr = r.ptr;
	r.ptr = ptr_tmp;
}


// comparison operators
	// shared_ptr vs. shared_ptr
template <class T, class U>
bool operator==(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return lhs.get()==rhs.get();
}
template <class T, class U>
bool operator!=(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return !(lhs == rhs);
}
template <class T, class U>
bool operator>(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return lhs.get() > rhs.get();
}
template <class T, class U>
bool operator<(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return lhs.get() < rhs.get();
}
template <class T, class U>
bool operator>=(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return lhs.get() >= lhs.get();
}
template <class T, class U>
bool operator<=(const zs_shared_ptr<T>& lhs, const zs_shared_ptr<U>& rhs) {
	return lhs.get() <= lhs.get();
}
	// nullptr vs. shared_ptr
template <class T>
bool operator==(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return rhs.get() == nullptr;
}
template <class T>
bool operator!=(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return rhs.get() != nullptr;
}
template <class T>
bool operator>(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return (T*)(nullptr) > rhs.get();
}
template <class T>
bool operator<(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return (T*)(nullptr) < rhs.get();
}
template <class T>
bool operator>=(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return (T*)(nullptr) >= rhs.get();
}
template <class T>
bool operator<=(std::nullptr_t lhs, const zs_shared_ptr<T>& rhs) {
	return (T*)(nullptr) <= rhs.get();
}
	// shared_ptr vs. nullptr
template <class T>
bool operator==(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() == nullptr;
}
template <class T>
bool operator!=(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() != nullptr;
}
template <class T>
bool operator>(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() > (T*)(nullptr);
}
template <class T>
bool operator<(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() < (T*)(nullptr);
}
template <class T>
bool operator>=(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() >= (T*)(nullptr);
}
template <class T>
bool operator<=(const zs_shared_ptr<T>& lhs, std::nullptr_t rhs) {
	return lhs.get() <= (T*)(nullptr);
}


// output stream
template <class T>
std::ostream& operator<<(std::ostream& os, const zs_shared_ptr<T>& p) {
	os << p.get();
	return os;
}


// algorithm
namespace std {
	// global swap function
	template <class T>
	void swap(zs_shared_ptr<T>& lhs, zs_shared_ptr<T>& rhs) {
		lhs.swap(rhs);
	}
	// hasher
	template <class T>
	struct hash<zs_shared_ptr<T>> {
		static /*thread_local*/ hash<T*> hasher_object;
		size_t operator()(const zs_shared_ptr<T>& arg) const {
			return hasher_object(arg);
		}
	};
}
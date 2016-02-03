#pragma once

#include <deque>
#include <cstdint>
#include <limits>
#include <iterator>
#include <type_traits>


namespace exc {


/// <summary>
/// 
/// </summary>
class BinarySerializer {
private:
	template <class T>
	class iterator_base : public std::iterator<std::random_access_iterator_tag, T> {
	public:
		friend class BinarySerializer;

		iterator_base() {
			index = -1;
			parent = nullptr;
		}

		T& operator*() {
			return parent->operator[](index);
		}

		T* operator->() {
			return &parent->operator[](index);
		}

		bool operator==(const iterator_base& rhs) const {
			return (index == rhs.index) |
				(rhs.index < 0 & index < 0) |
				(rhs.index >= parent->Size() & index >= parent->Size());
		}
		bool operator!=(const iterator_base& rhs) const {
			return !(*this == rhs);
		}

		iterator_base& operator++() {
			++index;
			return *this;
		}
		iterator_base operator++(int) {
			auto copy = *this;
			++index;
			return copy;
		}

		// bidirectional
		iterator_base& operator--() {
			--index;
			return *this;
		}
		iterator_base operator--(int) {
			auto copy = *this;
			--index;
			return copy;
		}

		// random access
		iterator_base& operator+=(difference_type n) {
			index += n;
			return *this;
		}
		iterator_base& operator-=(difference_type n) {
			index -= n;
			return *this;
		}
		iterator_base operator+(difference_type n) const {
			iterator_base it(*this);
			return it += n;
		}
		iterator_base operator-(difference_type n) const {
			iterator_base it(*this);
			return it -= n;
		}
		difference_type operator-(const iterator_base& rhs) const {
			return rhs.index - index;
		}

		bool operator<(const iterator_base& rhs) const {
			return index < rhs.index;
		}
		bool operator>(const iterator_base& rhs) const {
			return index > rhs.index;
		}
		bool operator<=(const iterator_base& rhs) const {
			return index <= rhs.index;
		}
		bool operator>=(const iterator_base& rhs) const {
			return index >= rhs.index;
		}

		template <class = typename std::enable_if<!std::is_const<T>::value>::type>
		operator iterator_base<const T>() {
			iterator_base<const T> it;
			it.parent = parent;
			it.index = index;
			return it;
		}
	protected:
		typename std::conditional<std::is_const<T>::value, const BinarySerializer*, BinarySerializer*>::type parent;
		intptr_t index;
	};

	template <class T>
	friend iterator_base<T> operator+(typename iterator_base<T>::difference_type n, const iterator_base<T>& it);

public:
	using const_iterator = iterator_base<const uint8_t>;
	using iterator = iterator_base<uint8_t>;

public:
	// raw input
	void Insert(const_iterator where, uint8_t);
	void Insert(const_iterator where, uint8_t* data, size_t size);
	template <class Iter>
	void Insert(const_iterator where, Iter first, Iter last);

	void PushFront(uint8_t* data, size_t size);
	void PushFront(uint8_t);
	template <class Iter>
	void PushFront(Iter first, Iter last);

	void PushBack(uint8_t* data, size_t size);
	void PushBack(uint8_t);
	template <class Iter>
	void PushBack(Iter first, Iter last);

	uint8_t PopBack();

	void Erase(const_iterator where, size_t size = 1);
	void Erase(const_iterator first, const_iterator last);

	static constexpr intptr_t EndPosition() { return std::numeric_limits<ptrdiff_t>::max(); }
	static constexpr intptr_t BeginPosition() { return 0; }

	// misc
	size_t Size() const { return buffer.size(); }

	// element access
	uint8_t& operator[](size_t index);
	const uint8_t& operator[](size_t index) const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
private:
	std::deque<uint8_t> buffer;
};

template <class T>
inline BinarySerializer::iterator_base<T> operator+(
	typename BinarySerializer::iterator_base<T>::difference_type n,
	const BinarySerializer::iterator_base<T>& it)
{
	return it + n;
}


//------------------------------------------------------------------------------
// insert operators
//------------------------------------------------------------------------------

// overload for bool
BinarySerializer& operator << (BinarySerializer& s, bool v) {
	s.PushBack(v ? 1 : 0);
	return s;
}

// signed integer type
template <class T>
void InsertSerialized(BinarySerializer& s, const BinarySerializer::const_iterator& where,
					  T v,
					  typename std::enable_if<
					  std::is_integral<T>::value &&
					  std::is_signed<T>::value,
					  T>::type = T())
{
	uint8_t buffer[sizeof(T)];
	bool isNegative = v < 0;
	T absolute = std::abs(v);
	for (int i = 0; i < sizeof(T); i++) {
		buffer[i] = (absolute >> (8 * (sizeof(T) - i - 1)));
	}
	if (isNegative) {
		buffer[0] |= 0b1000'0000;
	}
	s.Insert(where, buffer, sizeof(buffer));
}

// unsigned integer type
template <class T>
void InsertSerialized(BinarySerializer& s, const BinarySerializer::const_iterator& where,
					  T v,
					  typename std::enable_if<
					  std::is_integral<T>::value &&
					  !std::is_signed<T>::value,
					  T>::type = T())
{
	uint8_t buffer[sizeof(T)];
	for (int i = 0; i < sizeof(T); i++) {
		buffer[i] = (v >> (8 * (sizeof(T) - i - 1)));
	}
	s.Insert(where, buffer, sizeof(buffer));
}

// enumeration type
template <class T>
void InsertSerialized(BinarySerializer& s, const BinarySerializer::const_iterator& where,
					  T v,
					  typename std::enable_if<
					  std::is_enum<T>::value,
					  T>::type = T())
{
	using Underlying = typename std::underlying_type<T>::type;
	InsertSerialized(s, where, (Underlying)v);
}

// overload for integral and enum types
template <class T, class = typename std::enable_if<std::is_integral<T>::value || std::is_enum<T>::value>::type>
BinarySerializer& operator << (BinarySerializer& s, T v)
{
	InsertSerialized(s, s.end(), v);
	return s;
};

// overload for floating point types
// implement proper conversion to IEEE-754 big-endian format
// function deleted for until then
BinarySerializer& operator << (BinarySerializer& s, float v) = delete;

BinarySerializer& operator << (BinarySerializer& s, double v) = delete;

BinarySerializer& operator << (BinarySerializer& s, long double v) = delete;


// automatically enable front insertion - that sounds bad...
template <class T>
BinarySerializer& operator >> (const T& v, BinarySerializer& s) {
	BinarySerializer s_tmp;
	s_tmp << v;
	s.PushFront(s_tmp.cbegin(), s_tmp.cend());
}



//------------------------------------------------------------------------------
// extract operators
//------------------------------------------------------------------------------

// overload for bool
BinarySerializer& operator >> (BinarySerializer& s, bool& v) {
	v = s.PopBack();
	return s;
}


// signed integer type
template <class T>
void ExtractSerialized(BinarySerializer::const_iterator where,
					   T& v,
					   typename std::enable_if<
					   std::is_integral<T>::value &&
					   std::is_signed<T>::value,
					   T>::type = T())
{
	T value = 0;
	uint8_t first = *where;
	bool isNegative = 0b1000'0000 & first;
	first &= 0b0111'1111;
	value += first << ((sizeof(value) - 1) * 8);

	++where;
	for (int i = 1; i < sizeof(T); ++i, ++where) {
		value += (*where) << ((sizeof(value) - 1 - i) * 8);
	}

	if (isNegative) {
		value = -value;
	}

	v = value;
}


// unsigned integer type
template <class T>
void ExtractSerialized(BinarySerializer::const_iterator where,
					   T& v,
					   typename std::enable_if<
					   std::is_integral<T>::value &&
					   !std::is_signed<T>::value,
					   T>::type = T())
{
	T value = 0;
	for (int i = 0; i < sizeof(T); ++i, ++where) {
		value += ((*where) << ((sizeof(value) - 1 - i) * 8));
	}
	v = value;
}

// enumeration type
// enumeration type
template <class T>
void ExtractSerialized(BinarySerializer::const_iterator where,
					   T& v,
					   typename std::enable_if<
					   std::is_enum<T>::value,
					   T>::type = T())
{
	using Underlying = typename std::underlying_type<T>::type;
	Underlying v_;
	ExtractSerialized(where, v_);
	v = (T)v_;
}


// overload for integral and enum types
template <class T, class = typename std::enable_if<std::is_integral<T>::value || std::is_enum<T>::value>::type>
BinarySerializer& operator >> (BinarySerializer& s, T& v)
{
	ExtractSerialized(s.begin(), v);
	return s;
};


// overload for floating point types
// implement proper conversion to IEEE-754 big-endian format
// function deleted for until then
BinarySerializer& operator >> (BinarySerializer& s, float& v) = delete;

BinarySerializer& operator >> (BinarySerializer& s, double& v) = delete;

BinarySerializer& operator >> (BinarySerializer& s, long double& v) = delete;





} // !namespace exc!
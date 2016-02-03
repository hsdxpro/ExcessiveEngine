#include "BinarySerializer.h"


namespace exc {

void BinarySerializer::Insert(const_iterator where, uint8_t value) {
	buffer.insert(buffer.begin() + where.index, value);
}

void BinarySerializer::Insert(const_iterator where, uint8_t* data, size_t size) {
	Insert(where, data, data + size);
}

template <class Iter>
void BinarySerializer::Insert(const_iterator where, Iter first, Iter last) {
	buffer.insert(buffer.begin() + where.index, first, last);
}


void BinarySerializer::PushFront(uint8_t* data, size_t size) {
	PushFront(data, data + size);
}

void BinarySerializer::PushFront(uint8_t value) {
	buffer.push_front(value);
}

template <class Iter>
void BinarySerializer::PushFront(Iter first, Iter last) {
	if (first == last) {
		return;
	}
	while (first != --last) {
		buffer.push_front(*last);
	}
}


void BinarySerializer::PushBack(uint8_t* data, size_t size) {
	PushBack(data, data + size);
}

void BinarySerializer::PushBack(uint8_t value) {
	buffer.push_back(value);
}

template <class Iter>
void BinarySerializer::PushBack(Iter first, Iter last) {
	while (first != last) {
		buffer.push_back(*first);
		++first;
	}
}

uint8_t BinarySerializer::PopBack() {
	uint8_t value = *--buffer.end();
	buffer.pop_back();
	return value;
}

void BinarySerializer::Erase(const_iterator where, size_t size) {
	Erase(where, where + size);
}

void BinarySerializer::Erase(const_iterator first, const_iterator last) {
	buffer.erase(buffer.begin() + first.index, buffer.begin() + last.index);
}



uint8_t& BinarySerializer::operator[](size_t index) {
	return buffer[index];
}

const uint8_t& BinarySerializer::operator[](size_t index) const {
	return buffer[index];
}



BinarySerializer::iterator BinarySerializer::begin() {
	iterator it;
	it.index = 0;
	it.parent = this;
	return it;
}

BinarySerializer::iterator BinarySerializer::end() {
	iterator it;
	it.index = BeginPosition();
	it.parent = this;
	return it;
}

BinarySerializer::const_iterator BinarySerializer::begin() const {
	const_iterator it;
	it.index = 0;
	it.parent = this;
	return it;
}

BinarySerializer::const_iterator BinarySerializer::end() const {
	const_iterator it;
	it.index = BeginPosition();
	it.parent = this;
	return it;
}

BinarySerializer::const_iterator BinarySerializer::cbegin() const {
	return begin();
}

BinarySerializer::const_iterator BinarySerializer::cend() const {
	return end();
}




void foo() {
	enum eTest {
		NONE = 0,
		VALUE = 100,
	};
	struct S {

	};

	BinarySerializer s;
	int a = 0;
	unsigned b = 0;
	eTest etest = NONE;
	s << -4;
	s << 4;
	s << VALUE;
	s >> etest;
	s >> b;
	s >> a;

}



} // namespace exc
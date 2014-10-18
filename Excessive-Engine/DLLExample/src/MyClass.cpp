#include "MyClass.h"
#include <new>


void MyClass::release() {
	delete this;
}

int MyClass::add(int a, int b) {
	return a + b;
}

int MyClass::sum(int* data, int count) {
	int sum = 0;
	for (int i = 0; i < count; i++) {
		sum += data[i];
	}
	return sum;
}



// create function implemented here: nope, implement it in its own file if writing real code
MyInterface* CreateMyClass() {
	return new (std::nothrow) MyClass;
}
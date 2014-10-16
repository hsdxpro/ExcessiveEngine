#include "tests.h"
#include <iostream>

using namespace std;


struct __Restrict {
};

class A {
public:
	struct rDesc : public __Restrict {
		typedef A _T;
	};
	A() {
		cout << "A" << endl;
	}
};

class B {
public:
	struct rDesc : public __Restrict {
		typedef B _T;
	};
	B() {
		cout << "B" << endl;
	}
};

class Alloc {
public:
	template <class T>
	void Add(T desc) {
		static_assert(is_base_of<__Restrict, T>::value, "This descriptor is not supported.");
		auto* p = new T::_T;
		delete p;
	}
};

int PetiTestGeneric() {
	cout << "peti tesztje fut" << endl;

	Alloc a;
	A::rDesc d1;
	B::rDesc d2;
	a.Add(d1);
	a.Add(d2);

	return 0;
}
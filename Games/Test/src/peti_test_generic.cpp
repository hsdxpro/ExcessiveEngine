#include "tests.h"
#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>
#include <chrono>
#include "BasicTypes.h"

#include <mymath/mymath.h>

using namespace std;

int PetiTestGeneric() {
	mm::mat4 m1(
		1, 2, 3, 4,
		1.1, 2.2, 3.3, 4.4,
		3.1415, 2.718, 6.626, 1.38,
		6.672, 6.0221, 9.81, 1.414
		);
	mm::mat4 m2(
		299, 792, 458, 0.1,
		1.732, 2.236, 2.449, 2.646,
		1.602, 9.109, 5940, 12756,
		8.314, 101.325, 9, 8
		);
	m1 = mm::transpose(m1);
	m2 = mm::transpose(m2);
	mm::mat4 r = m1*m2;
	cout << m1 << endl;
	cout << m2 << endl;
	cout << r[0][3] << endl;
	cout << r << endl;


	return 0;


	unordered_map<string, int> m;
	vector<string> v;

	mt19937 rne;
	uniform_int_distribution<u32> rng('a', 'z');
	for (int i = 0; i < 40; i++) {
		char name[9] = "--------";
		for (int i = 0; i < 8; i++) {
			name[i] = rng(rne);
		}
		v.push_back(name);
		m.insert(pair<string, int>(name, rng(rne)));
	}


	int a;
	auto startTime = chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000000; i++) {
		uniform_int_distribution<size_t> rns(0, v.size()-1);
		auto it = m.find(v[rns(rne)]);
		if (it != m.end()) {
			a = it->second;
		}
	}
	auto duration = chrono::high_resolution_clock::now() - startTime;


	cout << "Lookup took " << chrono::duration_cast<chrono::microseconds>(duration).count() << " us" << endl;
	cout << "\t" << (double)chrono::duration_cast<chrono::microseconds>(duration).count() / 1e6 << " us/lookup" << endl;
	cout << a << endl;

	return 0;
}
#include "tests.h"
#include <iostream>
#include <unordered_map>
#include <random>
#include <cstdint>
#include <vector>
#include <chrono>

using namespace std;

int PetiTestGeneric() {	
	unordered_map<string, int> m;
	vector<string> v;

	mt19937 rne;
	uniform_int_distribution<unsigned> rng('a', 'z');
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
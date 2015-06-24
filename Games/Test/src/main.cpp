/* USAGE

1. Add your function in tests.h
2. Implement your function in another .cpp
3. Launch compile and launch this

Choose which test you want to run.
Your choice is remembered, and automatically run next time. (written to test_*.cfg)
To change choice press SPACE while waiting for launch.

*/


#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "tests.h"
#include "BasicTypes.h"

#ifdef _MSC_VER
#define _WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#endif


using namespace std;
using TestFunctionT = int(*)(void);

static const char CONFIG_FILE[] = "test_D6772914-293D-45EB-B5B9-597E6C5D7387.cfg";


void Pause() {
#ifdef _MSC_VER
	cout << "Press any key to continue..." << endl;
	_getch();
#else
	cout << "Press ENTER to continue..." << endl;
	cin.get();
#endif
}

void ListTests() {
	cout << "The following tests are available:" << endl;
	for (auto test : test_cases) {
		cout << test.first << endl;
	}
}
string GetTestName() {
	string name;
	ListTests();
	if (test_cases.size() == 0) {
		return "";
	}
	while (true) {
		cout << "Test name: ";
		getline(cin, name);
		auto idx = name.find_first_not_of(' ');
		name = name.substr(idx);
		auto it = test_cases.find(name);
		if (it != test_cases.end()) {
			return it->first;
		}
	}
}

string ParseFile() {
	return "";
}

int main() {
	// check if any tests to run
	if (test_cases.size() == 0) {
		cout << "Sorry, there's no any test to run.";
		Pause();
		return 0;
	}

	// try open config file
	std::fstream conf_file;
	conf_file.open(CONFIG_FILE, ios::in);

	string test_name;
	// ask for a test if no config
	if (!conf_file.is_open()) {
		// Get a name
		test_name = GetTestName();
	}
	// parse last test from config file
	else {
		getline(conf_file, test_name);
		auto it = test_cases.find(test_name);
		// in case file test is no longer available
		if (it == test_cases.end()) {
			test_name = GetTestName();
		}
		else {
#ifdef _MSC_VER
			cout << "Launching " << test_name;
			GetAsyncKeyState(VK_SPACE);
			for (int i = 0; i < 3; i++) {
				cout << ".";
				this_thread::sleep_for(chrono::milliseconds(1000));
			}
			cout << endl;
			bool isKey = (u32(GetAsyncKeyState(VK_SPACE))) > 0;
			if (isKey) {
				test_name = GetTestName();
			}
#else
			string ans;
			do {
				cout << "Launch " << test_name << "? y/n ";
				Getline(cin, ans);
			} while (ans == "" || ans == "y" || ans == "n");
			if (ans == "n") {
				test_name = GetTestName();
			}
#endif
		}
	}

	// Get test function's Address	
	TestFunctionT test_func;
	auto it = test_cases.find(test_name);
	if (it == test_cases.end()) {
		cout << "Test was not found, program will exit." << endl;
		Pause();
		return 0;
	}
	test_func = it->second;

	cout << "Running test \"" << test_name << "\"..." << endl;
	cout << "-------------------------------------------------------------------" << endl;
	auto ret = test_func();

	cout << "\n-------------------------------------------------------------------" << endl;
	cout << "Test exited with code " << ret << "." << endl;

	// write last test's name to config file
	conf_file.close();
	conf_file.open(CONFIG_FILE, ios::out | ios::trunc);
	conf_file << test_name;

	return 0;
}
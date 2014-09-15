#include <iostream>
#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <conio.h>
#include "../DLLExample/src/MyInterface.h"

using namespace std;


int main() {
	// vars
	MyInterface* myInterface = nullptr; // ptr to instance
	MyInterface*(*CreateMyClass)() = nullptr; // func ptr to function in other dll

	// load DLL
	HMODULE hModule = LoadLibrary("DLLExample");
	if (hModule == INVALID_HANDLE_VALUE) {
		cout << "Failed to load dll" << endl;
		return 1;
	}
	// get function pointer to the create method
	CreateMyClass = (MyInterface*(*)())GetProcAddress(hModule, "CreateMyClass");
	if (CreateMyClass == nullptr) {
		cout << "Failed to get func ptr" << endl;
		return 2;
	}

	// create an instance
	myInterface = CreateMyClass();
	if (myInterface == nullptr) {
		cout << "Failed to create an instance" << endl;
		return 3;
	}


	// test our interface
	int testData[4] = {1, 2, 3, 4};
	int a = 5, b = 8;
	
	int r = myInterface->add(a, b);
	cout << "r = " << r << endl;

	int sum = myInterface->sum(testData, 4);
	cout << "sum = " << sum << endl;

	_getch();
	return 0;
}
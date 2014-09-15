#pragma once

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

class MyInterface;


extern "C"
EXPORT MyInterface* CreateMyClass();


class MyInterface {
public:
	virtual void release() = 0;

	virtual int add(int a, int b) = 0;
	virtual int sum(int* data, int count) = 0;
};
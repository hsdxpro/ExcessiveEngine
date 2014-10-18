#pragma once
#include "MyInterface.h"


class MyClass : public MyInterface {
public:
	void release() override;

	int add(int a, int b) override;
	int sum(int* data, int count) override;
};
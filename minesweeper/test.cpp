#include "LRandom.h"
#include <iostream>

int main() {
	LRandom a;
	int l = 1, r = 2;
	std::cout << a.randnum(l, r);
}
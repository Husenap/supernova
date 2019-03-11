#include <iostream>

#include <supernova/supernova-lib.h>

int main(int, char**) {
	auto supernova = std::make_unique<sn::supernova>();

	supernova->test();

	return 0;
}
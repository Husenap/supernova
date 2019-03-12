#include <iostream>

#include <supernova/supernova-lib.h>

int main(int, char**) {
	auto supernova = std::make_unique<snova::supernova>();

	if (supernova->init()) {
		supernova->run();
	} else {
		std::cerr << "Failed to init engine" << std::endl;
	}

	return 0;
}
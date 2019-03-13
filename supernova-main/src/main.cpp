#include <iostream>

#include <supernova/supernova-lib.h>
#include <stdexcept>

#include <logger.h>

int main(int, char**) {
	auto supernova = std::make_unique<snova::supernova>();

	try {
		supernova->run();
	} catch (const std::exception& e) {
		ERROR_LOG("Exception: %s", e.what());
		return 1;
	}

	return 0;
}
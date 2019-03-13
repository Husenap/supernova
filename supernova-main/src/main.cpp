#include <iostream>

#include <supernova/supernova-lib.h>
#include <stdexcept>

#include <logger.h>

int main(int, char**) {
	INFO_LOG("Application Starting...");

	auto supernova = std::make_unique<snova::supernova>();

	try {
		supernova->run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
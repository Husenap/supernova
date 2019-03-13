#include <iostream>

#include <supernova/supernova-lib.h>
#include <stdexcept>

#include <logger.h>

int main(int, char**) {
	INFO_LOG("Application Starting...");

	auto supernova = std::make_unique<snova::supernova>();

	supernova->run();

	return 0;
}
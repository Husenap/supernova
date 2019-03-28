#include "gtest/gtest.h"

#include "../precompiled.h"

TEST(logger, get){
	auto& lgr = snova::logger::get();

	EXPECT_NE(&lgr, nullptr);
}

TEST(logger, logging){
	INFO_LOG("Testing info log!");
	VERBOSE_LOG("Testing verbose log!");
	WARNING_LOG("Testing warning log!");
	ERROR_LOG("Testing error log!");
	try{
		FATAL_LOG("Testing fatal log!");
		FAIL() << "FATAL_LOG should throw an exception\n";
	}catch(std::runtime_error&){}
}

TEST(logger, set_log_level){
	LOG_LEVEL(snova::logger::level::all);
	LOG_LEVEL(snova::logger::level::info);
	LOG_LEVEL(snova::logger::level::verbose);
	LOG_LEVEL(snova::logger::level::warning);
	LOG_LEVEL(snova::logger::level::error);
	LOG_LEVEL(snova::logger::level::fatal);
}

TEST(logger, timer){
	START_TIMER(timer_id)
	//Do a bunch of stuff
	END_TIMER(timer_id)
}

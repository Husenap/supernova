#pragma once

//#ifndef NDEBUG

#include <chrono>
#include "../singleton/singleton.h"

namespace snova {
class logger : public singleton<logger> {
public:
	enum level : char {
		all,
		info,
		verbose,
		warning,
		error,
		fatal,
		count
	};

	static constexpr int message_color_index	= 1;
	static constexpr int file_color_index		= 2;
	static constexpr int info_color_index		= 3;
	static constexpr int verbose_color_index	= 4;
	static constexpr int warning_color_index	= 5;
	static constexpr int error_color_index		= 6;
	static constexpr int fatal_color_index		= 7;

public:
	logger();
	~logger(){}

	void log(level log_level, const char* file, long line, const char* function_name, const char* format_string, ...);
	void set_level(level log_level) { m_level = log_level; }

private:
	level m_level = level::all;
};
}

#define INFO_LOG(...) snova::logger::get().log(snova::logger::level::info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define VERBOSE_LOG(...) snova::logger::get().log(snova::logger::level::verbose, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define WARNING_LOG(...) snova::logger::get().log(snova::logger::level::warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROR_LOG(...) snova::logger::get().log(snova::logger::level::error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define FATAL_LOG(...) snova::logger::get().log(snova::logger::level::fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define LOG_LEVEL(LEVEL) snova::logger::get().set_level(LEVEL);

#define START_TIMER(ID) std::chrono::steady_clock::time_point ID = std::chrono::steady_clock::now();
#define END_TIMER(ID) INFO_LOG("'%s' took %f ms", #ID, static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - ID).count())*1000.0);

/*
#else

#define INFO_LOG(...)
#define VERBOSE_LOG(...)
#define WARNING_LOG(...)
#define ERROR_LOG(...)
#define FATAL_LOG(...)

#define LOG_LEVEL(LEVEL)

#define START_TIMER(ID)
#define END_TIMER(ID)

#endif
*/


#pragma once

#ifndef NDEBUG

#include <memory>
#include <string>

namespace snova {
class logger {
public:
	enum class level : char { All, Verbose, Info, Warning, Error, Fatal, Count };

public:
	logger() { }
	~logger() { }

	static void set_log_level(level aLogLevel);

	static logger& get();

	void log(level log_level,
			 const char* file,
			 long line,
			 const char* function_name,
			 const char* format_string,
			 ...);

private:
	static std::unique_ptr<logger> our_instance;

	level m_log_level;

	static constexpr uint32_t _buffer_size = 2048;
};
}  // namespace snova

#define VERBOSE_LOG(...) \
	snova::logger::get().log(snova::logger::level::Verbose, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define INFO_LOG(...) \
	snova::logger::get().log(snova::logger::level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define WARNING_LOG(...) \
	snova::logger::get().log(snova::logger::level::Warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROR_LOG(...) \
	snova::logger::get().log(snova::logger::level::Error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define FATAL_LOG(...) \
	snova::logger::get().log(snova::logger::level::Fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#else

#define VERBOSE_LOG(...)
#define INFO_LOG(...)
#define WARNING_LOG(...)
#define ERROR_LOG(...)
#define FATAL_LOG(...)

#endif
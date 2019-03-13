#pragma once

#ifndef NDEBUG

namespace snova
{
	class logger
	{
	public:
		enum level : char
		{
			All,
			Info,
			Verbose,
			Warning,
			Error,
			Fatal,
			Count
		};

	public:
		static logger& get() {
			static logger our_logger;
			return our_logger;
		}

		void log(level log_level, const char* file, long line, const char* function_name, const char* format_string, ...);
		void set_level(level log_level) { m_level = log_level; }

	private:
		logger(){}
		~logger(){} 

		level m_level = All;
	};
}

#define INFO_LOG(...) snova::logger::get().log(snova::logger::level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define VERBOSE_LOG(...) snova::logger::get().log(snova::logger::level::Verbose, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define WARNING_LOG(...) snova::logger::get().log(snova::logger::level::Warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROR_LOG(...) snova::logger::get().log(snova::logger::level::Error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define FATAL_LOG(...) snova::logger::get().log(snova::logger::level::Fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define LOG_LEVEL(LEVEL) snova::logger::get().set_level(LEVEL);
 
#define START_TIMER(ID) std::chrono::steady_clock::time_point ID = std::chrono::steady_clock::now();
#define END_TIMER(ID) INFO_LOG("\"%s\" took %f ms", #ID, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - ID).count()/1000.f);

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


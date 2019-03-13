#ifndef NDEBUG

#include "../include/logger.h"

#include <cstdarg>
#include <filesystem>
#include <iostream>

namespace snova {

std::unique_ptr<logger> logger::our_instance = nullptr;

static const char* const level_strings[] = {"", "Verbose", "Info", "Warning", "Error", "FATAL", ""};

logger& snova::logger::get() {
	if (!our_instance) {
		our_instance = std::make_unique<logger>();
		set_log_level(logger::level::All);
	}
	return *our_instance;
}

void logger::set_log_level(level aLogLevel) {
	get().m_log_level = aLogLevel;
}

void snova::logger::log(
	level log_level, const char* file, long line, const char* function_name, const char* format_string, ...) {
	namespace fs = std::filesystem;

	if ((char)log_level < (char)m_log_level) return;
	if (log_level == logger::level::All || log_level == logger::level::Count) return;

	static const constexpr int32_t MaxBufferSize = 4096;

	char buffer[MaxBufferSize];
	va_list args;
	va_start(args, format_string);
	vsprintf_s(buffer, format_string, args);
	va_end(args);

	std::string shortFile = file;
	if (shortFile.find_last_of("\\/") != std::string::npos) {
		shortFile = shortFile.substr(shortFile.find_last_of("\\/")+1);
	}

	std::string functionName(function_name);

	if (functionName.find("lambda") != std::string::npos) {
		functionName = functionName.substr(0, functionName.find("lambda") + 6) + ">";
	}

	std::string shortFuncName;

	size_t shorteningPos = functionName.find_last_of(":");
	if (shorteningPos != std::string::npos) {
		shortFuncName = functionName.substr(shorteningPos + 1);
	} else {
		shortFuncName = functionName;
	}

	printf_s("[%s]%s:%li:%s: %s\n",
			  level_strings[(char)log_level],
			  shortFile.c_str(),
			  line,
			  functionName.c_str(),
			  buffer);

}

}  // namespace snova

#endif
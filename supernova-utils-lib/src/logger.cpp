#ifndef NDEBUG
#include "../include/logger.h"

#include <cstdarg>
#include <filesystem>
#include <functional>
#include <string>

namespace snova {

static const char* const level_strings[logger::level::Count + 1] = { "", "---Info", "Verbose", "Warning", "--Error", "--Fatal", ""};

void logger::log(
	level log_level, const char* file, long line, const char* function_name, const char* format_string, ...) {
	if (log_level < m_level) return;
	static constexpr int MAXSIZE = 1024;
	char buffer1[MAXSIZE];

	va_list args;
	va_start(args, format_string);
	vsprintf_s(buffer1, format_string, args);
	// perror(buffer1);
	va_end(args);

	std::string file_name = std::filesystem::path(file).filename().string();
	std::string clean_function_name = function_name;

	if (clean_function_name.find(":<lambda") != std::string::npos) {
		clean_function_name =
			clean_function_name.substr(0, clean_function_name.find(":<lambda") - 1) + "<lambda>";
	}

	std::string short_function_name = clean_function_name;
	size_t shortening_pos = clean_function_name.find_last_of(":");
	if (shortening_pos != std::string::npos)
		short_function_name = clean_function_name.substr(shortening_pos + 1);

	char buffer2[MAXSIZE];

	sprintf_s(buffer2,
			  "[%s]%s(%s:%li): %s\n",
			  level_strings[log_level],
			  short_function_name.c_str(),
			  file_name.c_str(),
			  line,
			  buffer1);

	printf("%s", buffer2);

	if (log_level == Fatal) {
		throw std::runtime_error(buffer2);
	}
}
}  // namespace snova

#endif

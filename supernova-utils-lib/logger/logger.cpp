//#ifndef NDEBUG
#include "logger.h"

#include <cstdarg>
#include <filesystem>
#include <functional>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace snova {

static const char* const level_strings[logger::level::Count + 1] = {
	"", "---Info", "Verbose", "Warning", "--Error", "--Fatal", ""};

void logger::log(
	level log_level, const char* file, long line, const char* function_name, const char* format_string, ...) {
	if (log_level < m_level) return;
	static constexpr int MAXSIZE = 1024;
	char buffer1[MAXSIZE];

	va_list args;
	va_start(args, format_string);
	vsprintf_s(buffer1, format_string, args);
	//perror(buffer1);
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
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, int(log_level) + 2);
	printf("[%s]", level_strings[log_level]);
	SetConsoleTextAttribute(hConsole, file_color_index);
	printf("%s(%s:%li): ", short_function_name.c_str(), file_name.c_str(), line);
	SetConsoleTextAttribute(hConsole, message_color_index);
	printf("%s\n", buffer1);
#else
	printf("%s", buffer2);
#endif

	if (log_level == Fatal) {
		throw std::runtime_error(buffer2);
	}
}
logger::logger() {
#ifdef _WIN32
	HWND hwnd = GetConsoleWindow();

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 245, LWA_ALPHA);

	auto console = GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	csbi.cbSize = sizeof(_CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(console, &csbi);

	// Snazzy
	/*
	csbi.ColorTable[0] = 0x362a28;
	csbi.ColorTable[message_color_index] = 0xf0f1f1;
	csbi.ColorTable[file_color_index] = 0x8ef75a;
	csbi.ColorTable[info_color_index] = 0xfeed9a;
	csbi.ColorTable[verbose_color_index] = 0xffc757;
	csbi.ColorTable[warning_color_index] = 0x9df9f3;
	csbi.ColorTable[error_color_index] = 0xc16aff;
	csbi.ColorTable[fatal_color_index] = 0x575cff;
	*/
	// gruvbox
	csbi.ColorTable[0] = 0x282828;
	csbi.ColorTable[message_color_index] = 0xb4daeb;
	csbi.ColorTable[file_color_index] = 0x758392;
	csbi.ColorTable[info_color_index] = 0x98a584;
	csbi.ColorTable[verbose_color_index] = 0x7fbf8f;
	csbi.ColorTable[warning_color_index] = 0x41bcf9;
	csbi.ColorTable[error_color_index] = 0x2d80fc;
	csbi.ColorTable[fatal_color_index] = 0x3c4bf8;

	SetConsoleScreenBufferInfoEx(console, &csbi);
#endif
}
}  // namespace snova

//#endif

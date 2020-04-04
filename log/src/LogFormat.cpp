#include "LogFormat.h"

namespace HW {
	
	namespace Format {

		const std::string Color::COLOR_BLACK =			"\033[30m";
		const std::string Color::COLOR_RED =			"\033[31m";
		const std::string Color::COLOR_GREEN =			"\033[32m";
		const std::string Color::COLOR_YELLOW =			"\033[33m";
		const std::string Color::COLOR_BLUE =			"\033[34m";
		const std::string Color::COLOR_MAGENTA =		"\033[35m";
		const std::string Color::COLOR_CYAN =			"\033[36m";
		const std::string Color::COLOR_WHITE =			"\033[37m";
					 
		const std::string Color::COLOR_BOLD_BLACK =		"\033[1m\033[30m";
		const std::string Color::COLOR_BOLD_RED =		"\033[1m\033[31m";
		const std::string Color::COLOR_BOLD_GREEN =		"\033[1m\033[32m";
		const std::string Color::COLOR_BOLD_YELLOW =	"\033[1m\033[33m";
		const std::string Color::COLOR_BOLD_BLUE =		"\033[1m\033[34m";
		const std::string Color::COLOR_BOLD_MAGENTA =	"\033[1m\033[35m";
		const std::string Color::COLOR_BOLD_CYAN =		"\033[1m\033[36m";
		const std::string Color::COLOR_BOLD_WHITE =		"\033[1m\033[37m";
		
		const std::string Color::COLOR_DEFAULT =		"\033[39m";
		const std::string Color::RESET_ENDLINE =		"\033[0m";

	}

}


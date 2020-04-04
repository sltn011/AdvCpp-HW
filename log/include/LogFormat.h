#ifndef HW_LOGFORMAT_H
#define HW_LOGFORMAT_H

#include <map>
#include <string>

namespace HW {

    namespace Format {

		class Color {
		public:
			static const std::string COLOR_BLACK;
			static const std::string COLOR_RED;
			static const std::string COLOR_GREEN;
			static const std::string COLOR_YELLOW;
			static const std::string COLOR_BLUE;
			static const std::string COLOR_MAGENTA;
			static const std::string COLOR_CYAN;
			static const std::string COLOR_WHITE;

			static const std::string COLOR_BOLD_BLACK;
			static const std::string COLOR_BOLD_RED;
			static const std::string COLOR_BOLD_GREEN;
			static const std::string COLOR_BOLD_YELLOW;
			static const std::string COLOR_BOLD_BLUE;
			static const std::string COLOR_BOLD_MAGENTA;
			static const std::string COLOR_BOLD_CYAN;
			static const std::string COLOR_BOLD_WHITE;
			
			static const std::string COLOR_DEFAULT;
			static const std::string RESET_ENDLINE;
		};

    } // Format

} // HW

#endif // HW_LOGFORMAT_H
#ifndef HW_LOGFORMAT_H
#define HW_LOGFORMAT_H
#include <map>
#include <string>

namespace HW {

    namespace Format {

        namespace Color {
            inline const std::string BLACK =            "\033[30m";
            inline const std::string RED =              "\033[31m";
            inline const std::string GREEN =            "\033[32m";
            inline const std::string YELLOW =           "\033[33m";
            inline const std::string BLUE =             "\033[34m";
            inline const std::string MAGENTA =          "\033[35m";
            inline const std::string CYAN =             "\033[36m";
            inline const std::string WHITE =            "\033[37m";

            inline const std::string BOLD_BLACK =       "\033[1m\033[30m";
            inline const std::string BOLD_RED =         "\033[1m\033[31m";
            inline const std::string BOLD_GREEN =       "\033[1m\033[32m";
            inline const std::string BOLD_YELLOW =      "\033[1m\033[33m";
            inline const std::string BOLD_BLUE =        "\033[1m\033[34m";
            inline const std::string BOLD_MAGENTA =     "\033[1m\033[35m";
            inline const std::string BOLD_CYAN =        "\033[1m\033[36m";
            inline const std::string BOLD_WHITE =       "\033[1m\033[37m";

            inline const std::string DEFAULT =          "\033[39m";
            inline const std::string RESET_ENDLINE =    "\033[0m";
        };

    } // Format

} // HW

#endif // HW_LOGFORMAT_H
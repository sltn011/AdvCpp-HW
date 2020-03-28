
namespace HW {

    namespace Format {

        struct LogFormat {
            const char *TRACE_COLOR;
		    const char *DEBUG_COLOR;
		    const char *INFO_COLOR;
		    const char *WARN_COLOR;
		    const char *ERROR_COLOR;
		    const char *FATAL_COLOR;
        };

        static constexpr const char *COLOR_RED = "\033[31m";
        static constexpr const char *COLOR_YELLOW = "\033[33m";
        static constexpr const char *COLOR_GREEN = "\033[32m";
        static constexpr const char *COLOR_BLACK = "\033[30m";
        static constexpr const char *COLOR_WHITE = "\033[97m";
        static constexpr const char *COLOR_DEFAULT = "\033[39m";

        static constexpr const char *RESET_ENDLINE = "\033[0m";

    } // Format

} // HW
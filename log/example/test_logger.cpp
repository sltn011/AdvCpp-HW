#include "log.h"

int main() {

	HW::Format::LogFormat format{};
	format.TRACE_COLOR = HW::Format::COLOR_WHITE;
	format.DEBUG_COLOR = HW::Format::COLOR_WHITE;
	format.INFO_COLOR = HW::Format::COLOR_GREEN;
	format.WARN_COLOR = HW::Format::COLOR_YELLOW;
	format.ERROR_COLOR = HW::Format::COLOR_RED;
	format.FATAL_COLOR = HW::Format::COLOR_RED;

	auto stdout_log = HW::create_stdout_logger(HW::Level::ALL, &format);
	HW::Logger::get_instance()->set_global_logger(std::move(stdout_log));
	HW::trace("TRACE: Stdout");
	HW::debug("DEBUG: Stdout");
	HW::info("INFO: Stdout");
	HW::warn("WARN: Stdout");
	HW::error("ERROR: Stdout");
	HW::fatal("FATAL: Stdout");
	std::cout << std::endl;

	HW::Logger::get_instance()->set_global_logger(HW::create_stderr_logger(HW::Level::INFO, &format));
	HW::trace("TRACE: Stderr");
	HW::debug("DEBUG: Stderr");
	HW::info("INFO: Stderr");
	HW::warn("WARN: Stderr");
	HW::error("ERROR: Stderr");
	HW::fatal("FATAL: Stderr");


	HW::Logger::get_instance()->set_global_logger(HW::create_file_logger("log.txt", HW::Level::ALL));
	HW::trace("TRACE: File");
	HW::debug("DEBUG: File");
	HW::info("INFO: File");
	HW::warn("WARN: File");
	HW::error("ERROR: File");
	HW::fatal("FATAL: File");

	return 0;
}
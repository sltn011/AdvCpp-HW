#include "log.h"

int main() {

	HW::Format::LogFormat format;

	//HW::Format::fill_format_with(format, HW::Format::Color::DEFAULT); используется для заполнения 

	format[HW::Level::TRACE] = HW::Format::Color::GREEN;
	format[HW::Level::DEBUG] = HW::Format::Color::BOLD_GREEN;
	format[HW::Level::ERROR] = HW::Format::Color::RED;
	format[HW::Level::FATAL] = HW::Format::Color::BOLD_RED;
	
	HW::Logger::get_instance().set_global_logger(HW::create_stdout_logger(HW::Level::ALL, format));
	HW::trace("TRACE: Stdout");
	HW::debug("DEBUG: Stdout");
	HW::info("INFO: Stdout");
	HW::warn("WARN: Stdout");
	HW::error("ERROR: Stdout");
	HW::fatal("FATAL: Stdout");
	std::cout << std::endl;
	
	HW::Logger::get_instance().set_global_logger(HW::create_stderr_logger(HW::Level::ERROR, format));
	HW::trace("TRACE: Stderr");
	HW::debug("DEBUG: Stderr");
	HW::info("INFO: Stderr");
	HW::warn("WARN: Stderr");
	HW::error("ERROR: Stderr");
	HW::fatal("FATAL: Stderr");
	
	HW::Logger::get_instance().set_global_logger(HW::create_file_logger("log.txt", HW::Level::INFO));
	HW::trace("TRACE: File");
	HW::debug("DEBUG: File");
	HW::info("INFO: File");
	HW::warn("WARN: File");
	HW::error("ERROR: File");
	HW::fatal("FATAL: File");

	return 0;
}
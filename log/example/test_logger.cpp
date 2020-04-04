#include "log.h"

int main() {

	HW::Format::LogFormat format;
	format.insert(std::make_pair(HW::Level::TRACE, HW::Format::Color::COLOR_GREEN));
	format.insert(std::make_pair(HW::Level::DEBUG, HW::Format::Color::COLOR_BOLD_GREEN));
	//format.insert(std::make_pair(HW::Level::INFO, HW::Format::Color::COLOR_YELLOW));
	//format.insert(std::make_pair(HW::Level::WARN, HW::Format::Color::COLOR_BOLD_YELLOW));
	format.insert(std::make_pair(HW::Level::ERROR, HW::Format::Color::COLOR_RED));
	format.insert(std::make_pair(HW::Level::FATAL, HW::Format::Color::COLOR_BOLD_RED));
	
	auto i = HW::create_stdout_logger(HW::Level::ALL, format);
	auto stdout_log = std::make_unique<HW::StdoutLogger>(i);
	HW::Logger::get_instance().set_global_logger(std::move(stdout_log));
	HW::trace("TRACE: Stdout");
	HW::debug("DEBUG: Stdout");
	HW::info("INFO: Stdout");
	HW::warn("WARN: Stdout");
	HW::error("ERROR: Stdout");
	HW::fatal("FATAL: Stdout");
	std::cout << std::endl;
	
	auto stderr_logger = std::make_unique<HW::StderrLogger>(HW::create_stderr_logger(HW::Level::ERROR, format));
	HW::Logger::get_instance().set_global_logger(std::move(stderr_logger));
	HW::trace("TRACE: Stderr");
	HW::debug("DEBUG: Stderr");
	HW::info("INFO: Stderr");
	HW::warn("WARN: Stderr");
	HW::error("ERROR: Stderr");
	HW::fatal("FATAL: Stderr");
	
	auto file_logger = std::make_unique<HW::FileLogger>(HW::create_file_logger("log.txt", HW::Level::INFO));
	HW::Logger::get_instance().set_global_logger(std::move(file_logger));
	HW::trace("TRACE: File");
	HW::debug("DEBUG: File");
	HW::info("INFO: File");
	HW::warn("WARN: File");
	HW::error("ERROR: File");
	HW::fatal("FATAL: File");

	return 0;
}
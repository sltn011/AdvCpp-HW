#include "log.h"

namespace HW {

	std::unique_ptr<BaseLogger> Logger::global_logger;

	Logger* Logger::get_instance() {
		static Logger log_instance;
		return &log_instance;
	}

	BaseLogger* Logger::get_global_logger() const noexcept {
		return global_logger.get();
	}

	void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) noexcept
	{
		get_instance()->global_logger.swap(logger);
	}

	std::unique_ptr<FileLogger> create_file_logger(const std::string & path, Level level, Format::LogFormat *format) 
	{
		return std::unique_ptr<FileLogger>(new FileLogger(path, level, format));
	}

	std::unique_ptr<StdoutLogger> create_stdout_logger(Level level, Format::LogFormat *format) 
	{
		return std::unique_ptr<StdoutLogger>(new StdoutLogger(level, format));
	}

	std::unique_ptr<StderrLogger> create_stderr_logger(Level level, Format::LogFormat *format) 
	{
		return std::unique_ptr<StderrLogger>(new StderrLogger(level, format));
	}

	void trace(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->trace(msg);
	}

	void debug(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->debug(msg);
	}

	void info(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->info(msg);
	}

	void warn(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->warn(msg);
	}

	void error(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->error(msg);
	}

	void fatal(const std::string &msg) noexcept
	{
		Logger::get_instance()->get_global_logger()->fatal(msg);
	}

} // HW
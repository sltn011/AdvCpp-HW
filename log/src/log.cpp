#include "log.h"

namespace HW {

	Logger::Logger() : global_logger{std::make_unique<StdoutLogger>(create_stdout_logger(Level::ALL)) } {}

	Logger& Logger::get_instance() {
		static Logger log_instance;
		return log_instance;
	}

	std::unique_ptr<BaseLogger>& Logger::get_global_logger() {
		return global_logger;
	}

	void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger)
	{
		get_instance().global_logger = std::move(logger);
	}

	FileLogger create_file_logger(const std::string & path, Level level)
	{
		return FileLogger(path, level);
	}

	StdoutLogger create_stdout_logger(Level level)
	{
		return StdoutLogger(level);
	}

	StdoutLogger create_stdout_logger(Level level, Format::LogFormat format)
	{
		return StdoutLogger(level, format);
	}

	StderrLogger create_stderr_logger(Level level)
	{
		return StderrLogger(level);
	}

	StderrLogger create_stderr_logger(Level level, Format::LogFormat format)
	{
		return StderrLogger(level, format);
	}

	void trace(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->trace(msg);
	}

	void debug(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->debug(msg);
	}

	void info(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->info(msg);
	}

	void warn(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->warn(msg);
	}

	void error(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->error(msg);
	}

	void fatal(const std::string &msg)
	{
		Logger::get_instance().get_global_logger()->fatal(msg);
	}

} // HW
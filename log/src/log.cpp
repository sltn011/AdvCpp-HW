#include "log.h"

namespace HW {

	Logger::Logger() : global_logger{std::unique_ptr<StdoutLogger>(new StdoutLogger(Level::ALL)) } {}

	Logger& Logger::get_instance() {
		static Logger log_instance;
		return log_instance;
	}

	BaseLogger& Logger::get_global_logger() {
		return *global_logger;
	}

	void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger)
	{
		if (logger != nullptr) {
			global_logger = std::move(logger);
		}
	}

	std::unique_ptr<FileLogger> create_file_logger(const std::string & path, Level level)
	{
		return std::unique_ptr<FileLogger>(new FileLogger(path, level));
	}

	std::unique_ptr<StdoutLogger> create_stdout_logger(Level level)
	{
		return std::unique_ptr<StdoutLogger>(new StdoutLogger(level));
	}

	std::unique_ptr<StdoutLogger> create_stdout_logger(Level level, Format::LogFormat format)
	{
		return std::unique_ptr<StdoutLogger>(new StdoutLogger(level, format));
	}

	std::unique_ptr<StderrLogger> create_stderr_logger(Level level)
	{
		return std::unique_ptr<StderrLogger>(new StderrLogger(level));
	}

	std::unique_ptr<StderrLogger> create_stderr_logger(Level level, Format::LogFormat format)
	{
		return std::unique_ptr<StderrLogger>(new StderrLogger(level, format));
	}

	void trace(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().trace(msg);
	}

	void debug(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().debug(msg);
	}

	void info(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().info(msg);
	}

	void warn(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().warn(msg);
	}

	void error(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().error(msg);
	}

	void fatal(const std::string &msg)
	{
		Logger::get_instance().get_global_logger().fatal(msg);
	}

} // HW
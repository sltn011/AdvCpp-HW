#ifndef HW_LOG_H
#define HW_LOG_H

#include "Loggers.h"
#include <memory>

namespace HW {

	class Logger { // Singleton 
	private:
		std::unique_ptr<BaseLogger> m_global_logger;
		Logger();
		~Logger() = default;
		Logger(const Logger &rhs) = delete;
		Logger& operator=(const Logger &rhs) = delete;
	public:
		static Logger &get_instance();
		BaseLogger &get_global_logger();
		void set_global_logger(std::unique_ptr<BaseLogger> logger);
	};

	std::unique_ptr<FileLogger> create_file_logger (const std::string &path, Level level);
	std::unique_ptr<StdoutLogger> create_stdout_logger(Level level);
	std::unique_ptr<StdoutLogger> create_stdout_logger(Level level, Format::LogFormat format);
	std::unique_ptr<StderrLogger> create_stderr_logger(Level level);
	std::unique_ptr<StderrLogger> create_stderr_logger(Level level, Format::LogFormat format);

	void trace(const std::string &msg);
	void debug(const std::string &msg);
	void info(const std::string &msg);
	void warn(const std::string &msg);
	void error(const std::string &msg);
	void fatal(const std::string &msg);

} // HW

#endif // HW_LOG_H

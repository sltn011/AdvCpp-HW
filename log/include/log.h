#ifndef HW_LOG_H
#define HW_LOG_H

#include "Loggers.h"
#include <memory>

namespace HW {

	class Logger { // Singleton 
	private:
		static std::unique_ptr<BaseLogger> global_logger;
		Logger() = default;
		~Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
	public:
		static Logger* get_instance();
		BaseLogger* get_global_logger() const noexcept;
		void set_global_logger(std::unique_ptr<BaseLogger> logger) noexcept;
	};



	std::unique_ptr<FileLogger> create_file_logger
	(const std::string & path, Level level = Level::OFF, Format::LogFormat *format = nullptr);

	std::unique_ptr<StdoutLogger> create_stdout_logger
	(Level level = Level::OFF, Format::LogFormat *format = nullptr);

	std::unique_ptr<StderrLogger> create_stderr_logger
	(Level level = Level::OFF, Format::LogFormat *format = nullptr);



	void trace(const std::string &msg) noexcept;
	void debug(const std::string &msg) noexcept;
	void info(const std::string &msg) noexcept;
	void warn(const std::string &msg) noexcept;
	void error(const std::string &msg) noexcept;
	void fatal(const std::string &msg) noexcept;

} // HW

#endif // HW_LOG_H

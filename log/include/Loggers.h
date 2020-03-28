#ifndef HW_LOGGERS_H
#define HW_LOGGERS_H


#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "LogFormat.h"

namespace HW {

	enum class Level {
		ALL,
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		OFF
	};

	class BaseLogger {
	protected:
		Level level_;
		Format::LogFormat *format_;

		BaseLogger(Level level, Format::LogFormat *format);
		virtual void log(const std::string &msg) noexcept = 0;

	public:
		virtual ~BaseLogger() noexcept;
		void trace(const std::string &msg) noexcept;
		void debug(const std::string &msg) noexcept;
		void info(const std::string &msg) noexcept;
		void warn(const std::string &msg) noexcept;
		void error(const std::string &msg) noexcept;
		void fatal(const std::string &msg) noexcept;
		void set_level(const Level level) noexcept;
		void set_format(const Format::LogFormat *format) noexcept;
		Format::LogFormat *get_format() const noexcept;
		Level level() const noexcept;
		virtual void flush() noexcept = 0;

	};

	class FileLogger : public BaseLogger {
	private:
		std::ofstream log_file;
		FileLogger(const std::string & path, Level level, Format::LogFormat *format);
		void log(const std::string &msg) noexcept override;

		friend std::unique_ptr<FileLogger> create_file_logger(const std::string & path, Level level, Format::LogFormat *format);

	public:
		void flush() noexcept override;
	};

	class StdoutLogger : public BaseLogger {
	private:
		StdoutLogger(Level level, Format::LogFormat *format);
		void log(const std::string &msg) noexcept override;

		friend std::unique_ptr<StdoutLogger> create_stdout_logger(Level level, Format::LogFormat *format);

	public:
		void flush() noexcept override;
	};

	class StderrLogger : public BaseLogger {
	private:
		StderrLogger(Level level, Format::LogFormat *format);
		void log(const std::string &msg) noexcept override;

		friend std::unique_ptr<StderrLogger> create_stderr_logger(Level level, Format::LogFormat *format);

	public:
		void flush() noexcept override;
	};

} // HW

#endif // HW_LOGGERS_H
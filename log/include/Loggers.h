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

	namespace Format {
		using LogFormat = std::map<HW::Level, std::string>;

		void fill_format_with(LogFormat &format, const std::string &color);
	}

	class BaseLogger {
	protected:
		Level m_level;

		BaseLogger(Level level);
		virtual void log(const std::string &msg, Level level) = 0;

	public:
		virtual ~BaseLogger() noexcept;
		void trace(const std::string &msg);
		void debug(const std::string &msg);
		void info(const std::string &msg);
		void warn(const std::string &msg);
		void error(const std::string &msg);
		void fatal(const std::string &msg);
		void set_level(const Level level);
		Level level() const;
		virtual void flush() = 0;

	};

	class FileLogger : public BaseLogger {
	private:
		std::ofstream m_log_file;

		FileLogger(const std::string & path, Level level);
		void log(const std::string &msg, Level level) override;

		friend FileLogger create_file_logger(const std::string & path, Level level);

	public:
		void flush() override;

	};

	class StdoutLogger : public BaseLogger {
	private:
		Format::LogFormat m_format;

		StdoutLogger(Level level);
		StdoutLogger(Level level, Format::LogFormat format);
		void log(const std::string &msg, Level level) override;

		friend StdoutLogger create_stdout_logger(Level level);
		friend StdoutLogger create_stdout_logger(Level level, Format::LogFormat format);

	public:
		void flush() override;
		void set_format(const Format::LogFormat format);
		Format::LogFormat get_format() const;
	};

	class StderrLogger : public BaseLogger {
	private:
		Format::LogFormat m_format;

		StderrLogger(Level level);
		StderrLogger(Level level, Format::LogFormat format);
		void log(const std::string &msg, Level level) override;

		friend StderrLogger create_stderr_logger(Level level);
		friend StderrLogger create_stderr_logger(Level level, Format::LogFormat format);

	public:
		void flush() override;
		void set_format(const Format::LogFormat format);
		Format::LogFormat get_format() const;
	};

} // HW

#endif // HW_LOGGERS_H
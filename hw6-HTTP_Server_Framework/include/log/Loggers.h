#ifndef HW_LOGGERS_H
#define HW_LOGGERS_H


#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>
#include "log/LogFormat.h"

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
		Level		m_level;
		std::mutex	m_mutex;
		virtual void log(const std::string &msg, Level level) = 0;

	public:
		BaseLogger(Level level);
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
		void log(const std::string &msg, Level level) override;

	public:
		FileLogger(const std::string & path, Level level);
		void flush() override;

	};

	class StdoutLogger : public BaseLogger {
	private:
		Format::LogFormat m_format;
		void log(const std::string &msg, Level level) override;

	public:
		StdoutLogger(Level level);
		StdoutLogger(Level level, Format::LogFormat format);
		void flush() override;
		void set_format(const Format::LogFormat format);
		Format::LogFormat get_format() const;
	};

	class StderrLogger : public BaseLogger {
	private:
		Format::LogFormat m_format;
		void log(const std::string &msg, Level level) override;

	public:
		StderrLogger(Level level);
		StderrLogger(Level level, Format::LogFormat format);
		void flush() override;
		void set_format(const Format::LogFormat format);
		Format::LogFormat get_format() const;
	};

} // HW

#endif // HW_LOGGERS_H
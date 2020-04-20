#include "Loggers.h"

namespace HW {

	namespace Format {
		void fill_format_with(LogFormat &format, const std::string &color) {

			int begin = static_cast<int>(Level::ALL) + 1;
			int end	= static_cast<int>(Level::OFF);

			for (begin; begin != end; ++begin) {
				Level level = static_cast<Level>(begin);
				if (format.find(level) == format.end()) {
					std::pair<Level, std::string> entry = std::make_pair(level, color);
					format.insert(entry);
				}
			}
		}
	}

	BaseLogger::BaseLogger(Level level) : m_level(level) {}

	BaseLogger::~BaseLogger() {}

	void BaseLogger::trace(const std::string &msg)
	{
		log(msg, Level::TRACE);
	}

	void BaseLogger::debug(const std::string &msg)
	{
		log(msg, Level::DEBUG);
	}

	void BaseLogger::info(const std::string &msg)
	{
		log(msg, Level::INFO);
	}

	void BaseLogger::warn(const std::string &msg)
	{
		log(msg, Level::WARN);
	}

	void BaseLogger::error(const std::string &msg)
	{
		log(msg, Level::ERROR);
	}

	void BaseLogger::fatal(const std::string &msg)
	{
		log(msg, Level::FATAL);
	}

	void BaseLogger::set_level(Level level)
	{
		m_level = level;
	}

	Level BaseLogger::level() const
	{
		return m_level;
	}



	FileLogger::FileLogger(const std::string &path, Level level) :
	BaseLogger(level), m_log_file(path, std::ios_base::app) {}

	void FileLogger::log(const std::string &msg, Level level) 
	{
		if (m_level <= level) {
			m_log_file << msg << std::endl;
		}
	}
		

	void FileLogger::flush()
	{
		m_log_file.flush();
	}



	StdoutLogger::StdoutLogger(Level level) : BaseLogger(level) {
		Format::fill_format_with(m_format, Format::Color::DEFAULT);
	}

	StdoutLogger::StdoutLogger(Level level, Format::LogFormat format) : BaseLogger(level), m_format{format} {
		Format::fill_format_with(m_format, Format::Color::DEFAULT);
	}

	void StdoutLogger::log(const std::string &msg, Level level)
	{
		if (m_level <= level) {
			auto it = m_format.find(level);
			std::cout << it->second << msg << Format::Color::RESET_ENDLINE << std::endl;
		}
	}

	void StdoutLogger::flush()
	{
		std::flush(std::cout);
	}

	void StdoutLogger::set_format(const Format::LogFormat format)
	{
		m_format = format;
	}

	Format::LogFormat StdoutLogger::get_format() const
	{
		return m_format;
	}


	StderrLogger::StderrLogger(Level level) : BaseLogger(level) {
		Format::fill_format_with(m_format, Format::Color::DEFAULT);
	}

	StderrLogger::StderrLogger(Level level, Format::LogFormat format) : BaseLogger(level), m_format{format} {
		Format::fill_format_with(m_format, Format::Color::DEFAULT);
	}

	void StderrLogger::log(const std::string &msg, Level level)
	{
		if (m_level <= level) {
			auto it = m_format.find(level);
			std::cerr << it->second << msg << Format::Color::RESET_ENDLINE << std::endl;
		}
	}

	void StderrLogger::flush()
	{
		std::flush(std::cerr);
	}

	void StderrLogger::set_format(const Format::LogFormat format)
	{
		m_format = format;
	}

	Format::LogFormat StderrLogger::get_format() const
	{
		return m_format;
	}

} // HW



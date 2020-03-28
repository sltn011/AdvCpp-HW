#include "Loggers.h"

namespace HW {

	BaseLogger::BaseLogger(Level level, Format::LogFormat *format) : level_(level), format_(format) {}

	BaseLogger::~BaseLogger() {}

	void BaseLogger::trace(const std::string & msg) noexcept
	{
		if (level_ <= Level::TRACE) {
			if (!format_ || !(format_->TRACE_COLOR)) {
				log(msg);
			} else {
				log(format_->TRACE_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::debug(const std::string & msg) noexcept
	{
		if (level_ <= Level::DEBUG) {
			if (!format_ || !(format_->DEBUG_COLOR)) {
				log(msg);
			} else {
				log(format_->DEBUG_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::info(const std::string & msg) noexcept
	{
		if (level_ <= Level::INFO) {
			if (!format_ || !(format_->INFO_COLOR)) {
				log(msg);
			} else {
				log(format_->INFO_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::warn(const std::string & msg) noexcept
	{
		if (level_ <= Level::WARN) {
			if (!format_ || !(format_->WARN_COLOR)) {
				log(msg);
			} else {
				log(format_->WARN_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::error(const std::string & msg) noexcept
	{
		if (level_ <= Level::ERROR) {
			if (!format_ || !(format_->ERROR_COLOR)) {
				log(msg);
			} else {
				log(format_->ERROR_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::fatal(const std::string & msg) noexcept
	{
		if (level_ <= Level::FATAL) {
			if (!format_ || !(format_->FATAL_COLOR)) {
				log(msg);
			} else {
				log(format_->FATAL_COLOR + msg + Format::RESET_ENDLINE);
			}
		}
	}

	void BaseLogger::set_level(Level level) noexcept
	{
		level_ = level;
	}

	void BaseLogger::set_format(const Format::LogFormat *format) noexcept
	{
		format_ = const_cast<Format::LogFormat*>(format);
	}

	Format::LogFormat *BaseLogger::get_format() const noexcept
	{
		return format_;
	}

	Level BaseLogger::level() const noexcept
	{
		return level_;
	}



	FileLogger::FileLogger(const std::string & path, Level level, Format::LogFormat *format) : 
	BaseLogger(level, format), log_file(path, std::ios_base::app) {}

	void FileLogger::log(const std::string & msg) noexcept
	{
		log_file << msg << std::endl;
	}

	void FileLogger::flush() noexcept
	{
		log_file.flush();
	}



	StdoutLogger::StdoutLogger(Level level, Format::LogFormat *format) : BaseLogger(level, format) {}

	void StdoutLogger::log(const std::string & msg) noexcept
	{
		std::cout << msg << std::endl;
	}

	void StdoutLogger::flush() noexcept
	{
		std::flush(std::cout);
	}



	StderrLogger::StderrLogger(Level level, Format::LogFormat *format) : BaseLogger(level, format) {}

	void StderrLogger::log(const std::string & msg) noexcept
	{
		std::cerr << msg << std::endl;
	}

	void StderrLogger::flush() noexcept
	{
		std::flush(std::cerr);
	}

} // HW



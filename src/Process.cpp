#include "Process.h"

namespace HW {

	Process::Process(const std::string &procPath, std::vector<std::string> &argv) {
		m_path = procPath;
		int pipe_in[2], pipe_out[2];

		if (pipe(pipe_in) < 0) {
			throw HW::DescriptorError("Failed to create pipe to process!");
		}

		if (pipe(pipe_out) < 0) {
			throw HW::DescriptorError("Failed to create pipe out of process!");
		}

		m_pid = fork();
		if (m_pid < 0) {
			throw HW::ProcessError("Failed to create process!");
		}
		else if (!m_pid) {
			if (dup2(pipe_in[0], STDIN_FILENO) < 0) {
				throw HW::DescriptorError("Failed to duplicate file descriptor in!");
			}
			if (dup2(pipe_out[1], STDOUT_FILENO) < 0) {
				throw HW::DescriptorError("Failed to duplicate file descriptor out!");
			}
			::close(pipe_in[0]);
			::close(pipe_in[1]);
			::close(pipe_out[0]);
			::close(pipe_out[1]);

			std::vector<char*> procArgs;
			procArgs.push_back(const_cast<char*>(m_path.c_str()));
			for (size_t i = 0; i < argv.size(); ++i) {
				procArgs.push_back(const_cast<char*>(argv[i].c_str()));
			}
			procArgs.push_back(nullptr);

			if (execvp(m_path.c_str(), procArgs.data()) < 0) {
				throw HW::ProcessError("Failed to launch process " + m_path);
			}
		}
		else {
			::close(pipe_in[0]);
			::close(pipe_out[1]);
			m_fdIn.setFD(pipe_in[1]);
			m_fdOut.setFD(pipe_out[0]);
		}
	}

	Process::~Process() noexcept {
		if (m_pid) {
			if (waitpid(m_pid, nullptr, 0) < 0) {
				std::cerr << "Some error happened" << std::endl;
			}
		}
	}

	void Process::abort() {
		if (m_pid) {
			kill(m_pid, SIGTERM);
		}
	}

	size_t Process::write(const void *data, size_t len) {
		if (!isWritable()) {
			return 0;
		}
		ssize_t written = ::write(m_fdIn.getFD(), data, len);
		if (written < 0) {
			throw HW::IOError("Error writing to process!");
		}
		else {
			return static_cast<size_t>(written);
		}
	}

	void Process::writeExact(const void *data, size_t len) {
		if (!isWritable()) {
			throw HW::DescriptorError("Write stream is closed!");
		}
		size_t total_written = 0;
		while (total_written != len) {
			total_written += write(static_cast<const uint8_t*>(data) + total_written, len - total_written);
		}
	}

	size_t Process::read(void *data, size_t len) {
		if (!isReadable()) {
			return 0;
		}
		ssize_t recieved = ::read(m_fdOut.getFD(), data, len);
		if (recieved < 0) {
			throw HW::IOError("Error reading from process!");
		}
		else {
			return static_cast<size_t>(recieved);
		}
	}

	void Process::readExact(void *data, size_t len) {
		if (!isReadable()) {
			throw HW::DescriptorError("Read stream is closed!");
		}
		size_t total_read = 0;
		while (total_read != len) {
			size_t recieved = read(static_cast<uint8_t*>(data) + total_read, len - total_read);
			if (recieved == 0) {
				throw HW::IOError("End of file reached before reading required number of bytes!");
			}
			total_read += recieved;
		}
	}

	bool Process::isReadable() const {
		return m_fdOut.isOpened();
	}

	bool Process::isWritable() const {
		return m_fdIn.isOpened();
	}

	void Process::closeStdin() {
		m_fdIn.close();
	}

	void Process::close() {
		if (isWritable()) {
			m_fdIn.close();
		}
		if (isReadable()) {
			m_fdOut.close();
		}
	}

	int Process::getPID() const {
		return m_pid;
	}

	std::string Process::procName() const {
		return m_path;
	}

} // HW


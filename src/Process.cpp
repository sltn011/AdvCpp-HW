#include "Process.h"

HW::Process::Process(const std::string & procPath, std::vector<std::string> & argv) {
    path = procPath;
    int pipe_in[2], pipe_out[2];

    if (pipe(pipe_in) < 0) {
        throw HW::DescriptorError("Failed to create pipe to process!");
    }

    if (pipe(pipe_out) < 0) {
        throw HW::DescriptorError("Failed to create pipe out of process!");
    }

    pid = fork();
    if (pid < 0) {
        throw HW::ProcessError("Failed to create process!");
    } else if (!pid) {
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
        procArgs.push_back(const_cast<char*>(path.c_str()));
        for (size_t i = 0; i < argv.size(); ++i) {
            procArgs.push_back(const_cast<char*>(argv[i].c_str()));
        }
        procArgs.push_back(nullptr);

       if (execvp(path.c_str(), procArgs.data()) < 0) {
           throw HW::ProcessError("Failed to launch process " + path);
       }
    } else {
        ::close(pipe_in[0]);
        ::close(pipe_out[1]);
        fd_in.setFD(pipe_in[1]);
        fd_out.setFD(pipe_out[0]);
    }
}

HW::Process::~Process() noexcept {
    if (pid) {
        if (waitpid(pid, nullptr, 0) < 0) {
            std::cerr << "Some error happened" << std::endl;
        }
    }
}

void HW::Process::abort() {
    if (pid) {
        kill(pid, SIGTERM);
    }
}

size_t HW::Process::write(const void* data, size_t len) {
    if (!isWritable()) {
        return 0;
    }
    ssize_t written = ::write(fd_in.getFD(), data, len);
    if (written < 0) {
        throw HW::IOError("Error writing to process!");
    } else {
        return static_cast<size_t>(written);
    }
}

void HW::Process::writeExact(const void* data, size_t len) {
    if (!isWritable()) {
        throw HW::DescriptorError("Write stream is closed!");
    }
    size_t total_written = 0;
    while(total_written != len) {
        total_written += write(static_cast<const uint8_t*>(data) + total_written, len - total_written);
    }
}

size_t HW::Process::read(void* data, size_t len) {
    if (!isReadable()) {
        return 0;
    }
    ssize_t recieved = ::read(fd_out.getFD(), data, len);
    if (recieved < 0) {
        throw HW::IOError("Error reading from process!");
    } else {
        return static_cast<size_t>(recieved);
    }
}

void HW::Process::readExact(void* data, size_t len) {
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

bool HW::Process::isReadable() const noexcept {
    return fd_out.isOpened();
}

bool HW::Process::isWritable() const noexcept {
    return fd_in.isOpened();
}

void HW::Process::closeStdin() {
    fd_in.close();
}

void HW::Process::close() {
    if (isWritable()) {
        fd_in.close();
    }
    if (isReadable()) {
        fd_out.close();
    }
}

int HW::Process::getPID() const noexcept {
    return pid;
}

std::string HW::Process::procName() const noexcept {
    return path;
}
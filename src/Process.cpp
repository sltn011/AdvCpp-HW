#include "Process.h"

Process::Process(const std::string & path, char *argv[]) {
    this->path = path;

    if (pipe(pipe_in) < 0) {
        throw std::runtime_error("Failed to create pipe_in!");
    }
    writeableTo = true;

    if (pipe(pipe_out) < 0) {
        throw std::runtime_error("Failed to create pipe_out!");
    }
    readableFrom = true;

    pid = fork();
    if (pid < 0) {
        throw std::runtime_error("Failed to create process!");
    } else if (!pid) {
        if (dup2(pipe_in[0], STDIN_FILENO) < 0) {
            throw std::runtime_error("Failed to duplicate file descriptor in!");
        }
        if (dup2(pipe_out[1], STDOUT_FILENO) < 0) {
            throw std::runtime_error("Failed to duplicate file descriptor out!");
        }
        ::close(pipe_in[1]);
        ::close(pipe_out[0]);
        execvp(path.c_str(), argv);
    } else {
        ::close(pipe_in[0]);
        ::close(pipe_out[1]);
    }
}

Process::~Process() {
    if (pid) {
        if (wait(nullptr) < 0) {
            std::cerr << "Some error happened" << std::endl;
        }
    }
}

void Process::abort() {
    if (pid) {
        kill(pid, SIGKILL);
    }
}

size_t Process::write(const void* data, size_t len) {
    if (!writeableTo) {
        return 0;
    } else return ::write(pipe_in[1], data, len);
}

void Process::writeExact(const void* data, size_t len) {
    if (!writeableTo) {
        throw std::runtime_error("Write stream is closed!");
    }
    for (size_t i = 0; i < len; ++i) {
        if (::write(pipe_in[1], static_cast<const uint8_t*>(data) + i, 1) != 1) {
            --i;
        }
    }
}

size_t Process::read(void* data, size_t len) {
    if (!readableFrom) {
        return 0;
    } else return ::read(pipe_out[0], data, len);
}

void Process::readExact(void* data, size_t len) {
    if (!readableFrom) {
        throw std::runtime_error("Read stream is closed!");
    } 
    for (size_t i = 0; i < len; ++i) {
        if (::read(pipe_out[0], static_cast<uint8_t*>(data) + i, 1) != 1) {
            --i;
        }
    }
}

bool Process::isReadable() const {
    return readableFrom;
}

bool Process::isWritable() const {
    return writeableTo;
}

void Process::closeStdin() {
    ::close(pipe_in[1]);
    writeableTo = false;
}

void Process::close() {
    if (writeableTo) {
        ::close(pipe_in[1]);
        writeableTo = false;
    }
    if (readableFrom) {
        ::close(pipe_out[0]);
        readableFrom = false;
    }
}

int Process::getPID() const {
    return pid;
}

std::string Process::procName() const {
    return path;
}
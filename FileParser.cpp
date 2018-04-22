#include <fstream>
#include <iostream>

#include "FileParser.hpp"
#include "Config.hpp"

template <class T>
void read_property(std::ifstream& source, T& target) {
    if (source.eof()) {
        _is_valid = false;
        return;
    }
    source >> target;
}

void parse_key(std::ifstream& source) {
    std::string read_key;
    source >> read_key;
    switch (read_key) {
        case config::ADDRESS:
            read_property(source, _address);
            break;
        case config::ROOT_FOLDER:
            read_property(source, _root_folder);
            break;
        case config::QUEUE_SIZE:
            read_property(source, _queue_size);
            break;
        case config::PORT:
            read_property(source, _port);
            break;
        case config::CPU_COUNT:
            read_property(source, _cpu_count);
            break;
        case config::THREAD_COUNT:
            read_property(source, _threads_count);
            break;
        default:
           is_valid = false;
           std::cerr << config::UNKNOWN_KEY_ERROR << read_key << std::endl;
    }
}

FileParser::FileParser(const std::string& config_filepath) {
    read(config_filepath + config::CONFIG_FILENAME);
}

FileParser::~FileParser() {}

std::uint_16t FileParser::port() const {
    return _port;
}

std::uint_16t FileParser::cpu_count() const {
    return _cpu_count;
}

std::uint_16t FileParser::threads_count() const {
    return _threads_count;
}

size_t FileParser::queue_size() const {
    return _queue_size;
}

const std::string& FileParser::address() const {
    return _address;
}

const std::string& FileParser::root_folder() const {
    return _root_folder;
}

void FileParser::read(const std::string& path) {
    std::ifstream config_file;
    config_file.open(path);
    _is_valid = !config_file.eof();
    while (_is_valid && !config_file.eof()) {
        parse_key(config_file);
    }
    if (!_is_valid) {
        init_defaults();
    }
}

void FileParser::init_defaults() {
    _port = config::DEFAULT_PORT;
    _cpu_count = config::DEFAULT_CPU_COUNT;
    _threads_count = config::DEFAULT_THREAD_COUNT;

    _queue_size = config::DEFAULT_QUEUE_SIZE;

    _address = config::DEFAULT_ADDRESS;
    _root_folder = config::DEFAULT_ROOT_FOLDER;

    _is_valid = true;
}

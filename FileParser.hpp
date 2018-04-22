#ifndef FILE_PARSER_HPP
#define FILE_PARSER_HPP

#include <cstdint>
#include <fstream>
#include <string>

#include "Config.hpp"

class FileParser {
    private:
        std::uint16_t _port;
        std::uint16_t _cpu_count;
        std::uint16_t _threads_count;
        
        size_t _queue_size;

        std::string _address;
        std::string _root_folder;

        bool _is_valid;
    
        void read(const std::string& path);
        void init_defaults();
    
        template <class T>
        void read_property(std::ifstream& source, T& target) {
            if (source.eof()) {
                _is_valid = false;
                return;
            }
            source >> target;
        }

        void parse_key(std::ifstream& source);

    public:
        FileParser(const std::string& config_filepath);
        ~FileParser();

        std::uint16_t port() const;
        std::uint16_t cpu_count() const;
        std::uint16_t threads_count() const;

        size_t queue_size() const;

        const std::string& address() const;
        const std::string& root_folder() const;

        bool is_valid() const;
};

#endif

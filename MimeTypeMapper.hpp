#ifndef MIME_TYPE_MAPPER_HPP
#define MIME_TYPE_MAPPER_HPP

#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> mime_type_map_t;

const std::string UNKNOWN_MIME_TYPE = "unknown";

class MimeTypeMapper {
    private:
        mime_type_map_t _mime_map;

        void init_mime_map();

    public:
        MimeTypeMapper();
        ~MimeTypeMapper();

        bool extension_exists(const std::string& extension) const;
        const std::string& get_mime_type_for_extension(const std::string& extension) const;
};

#endif

#include "MimeTypeMapper.hpp"
#include "SupportedMimeTypes.hpp"
#include "SupportedFileExtensions.hpp"

MimeTypeMapper::MimeTypeMapper() {
    init_mime_map();
}

MimeTypeMapper::~MimeTypeMapper() {}

void MimeTypeMapper::init_mime_map() {
    _mime_map[TXT] = TEXT_PLAIN;
    _mime_map[HTML] = TEXT_HTML;
    _mime_map[CSS] = TEXT_CSS;

    _mime_map[JAVASCRIPT] = APP_JS;
    _mime_map[FLASH] = APP_FLASH;

    _mime_map[JPG] = IMAGE_JPEG;
    _mime_map[JPEG] = IMAGE_JPEG;
    _mime_map[PNG] = IMAGE_PNG;
    _mime_map[GIF] = IMAGE_GIF;
}

bool MimeTypeMapper::extension_exists(const std::string& extension) const {
    return _mime_map.count(extension) > 0;
}

const std::string& MimeTypeMapper::get_mime_type_for_extension(const std::string& extension) const {
    if (extension_exists(extension)) {
        return _mime_map[extension];
    } else {
        return UNKNOWN_MIME_TYPE;
    }
}

#include <iostream>
#include <memory>
#include <string>

#include "Config.hpp"
#include "FileParser.hpp"
#include "HttpServer.hpp"

int main(int argc, char* argv[]) {
    std::string config_directory = argv[0];
    config_directory = config_directory.empty()
        ? config::DEFAULT_CONFIG_FILE_PATH : config_directory;

    std::unique_ptr<FileParser> config(new FileParser(config_directory));
    std::unique_ptr<HttpServer> server(new HttpServer(config->address(), config->port(),
        config->queue_size(), config->root_folder(), config->threads_count()));

    return 0;
}

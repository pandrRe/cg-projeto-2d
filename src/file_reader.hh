#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace CG {
    std::string read_file(const char* path) {
        std::stringstream stream;
        std::ifstream file;
        file.open(path);
        if (!file) {
            std::cerr << "File not found." << std::endl;
        }
        stream << file.rdbuf();
        file.close();
        return stream.str();
    }
}

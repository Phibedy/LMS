#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>

#include <lms/config.h>
#include <lms/extra/string.h>

namespace lms {

template<>
bool parse<std::string>(const std::string &src,
                                      std::string &dst) {
    dst = src;
    return true;
}

template<>
bool parse<bool>(const std::string &src, bool &dst) {
    if(src == "0" || src == "false") {
        dst = false;
        return true;
    }
    if(src == "1" || src == "true") {
        dst = true;
        return true;
    }
    return false;
}

void Config::load(std::istream &in) {
    std::string line;
    bool isMultiline = false;
    std::string lineBuffer;

    while(lms::extra::safeGetline(in, line)) {
        if(extra::trim(line).empty() || line[0] == '#') {
            // ignore empty lines and comment lines
            isMultiline = false;
        } else {
            bool isCurrentMultiline = line[line.size() - 1] == '\\';
            std::string normalizedLine = isCurrentMultiline ?
                        line.erase(line.size() - 1) : line;

            if(isMultiline) {
                lineBuffer += normalizedLine;
            } else {
                lineBuffer = normalizedLine;
            }

            isMultiline = isCurrentMultiline;
        }

        if(! isMultiline) {
            size_t index = lineBuffer.find_first_of('=');

            if(index != std::string::npos) {
                properties[extra::trim(lineBuffer.substr(0, index))]
                        = extra::trim(lineBuffer.substr(index+1));
            }
        }
    }
}

bool Config::loadFromFile(const std::string &path) {
    std::ifstream in(path);

    if(! in.is_open()) {
        return false;
    }

    load(in);
    in.close();
    return true;
}

bool Config::hasKey(const std::string &key) const {
    return properties.count(key) == 1;
}

bool Config::empty() const {
    return properties.empty();
}

template<>
void Config::set<std::string>(const std::string &key, const std::string &value) {
    properties[key] = value;
}

} // namespace lms

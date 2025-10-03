// Vibecoded this code here, sorry.. :(
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "hello_imgui/hello_imgui_logger.h"

// Decode function: parses the custom protocol string into a vector of strings
inline std::vector<std::string> guac_decode(const std::string& input) {
    HelloImGui::Log(HelloImGui::LogLevel::Debug, "GUAC: Decoding WS Message: \"%s\"", input.c_str());
    int pos = -1;
    std::vector<std::string> sections;

    while (true) {
        size_t len = input.find('.', pos + 1);

        if (len == std::string::npos)
            break;

        // Extract the length value as a string, then parse as int
        std::string lengthStr = input.substr(pos + 1, len - pos - 1);
        int strLen;
        try {
            strLen = std::stoi(lengthStr);
        }
        catch (...) {
            return {};
        }

        pos = strLen + len + 1;

        if (pos > input.size())
            return {};

        sections.push_back(input.substr(len + 1, strLen));

        char sep = (pos < input.size()) ? input[pos] : '\0';

        if (sep == ',')
            continue;
        else if (sep == ';')
            break;
        else
            return {};
    }
    return sections;
}

// Encode function: builds the custom protocol string from a vector of strings
inline std::string guac_encode(const std::vector<std::string>& strings) {
    std::ostringstream oss;
    for (size_t i = 0; i < strings.size(); ++i) {
        oss << strings[i].length() << "." << strings[i];
        if (i < strings.size() - 1)
            oss << ",";
        else
            oss << ";";
    }
    return oss.str();
}
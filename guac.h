// Vibecoded this code here, sorry.. :(
// TODO: Rewrite this from scratch without artificial unintelligence.
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "hello_imgui/hello_imgui_logger.h"

// Decode function: parses the custom protocol string into a vector of strings
inline std::vector<std::string> guac_decode(const std::string& input) {

    //Hacky fix to stop the client ui from crashing due to printing really long base64 data in the logs by filtering out sync and png.
    if (!input.starts_with("3.png") && !input.starts_with("4.sync"))
        HelloImGui::Log(HelloImGui::LogLevel::Debug, "GUAC: Encoded Message: \"%s\"", input.c_str());


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
        if (sep == ';')
	        break;
        return {};
    }

#ifndef NDEBUG

    //Hacky fix to stop the client ui from crashing due to printing really long base64 data in the logs by filtering out png and sync
    if (sections[0] != "png" && sections[0] != "sync"){
        std::string s;
        for (const auto& piece : sections) s += " [" + piece + "] ";

        HelloImGui::Log(HelloImGui::LogLevel::Debug, "GUAC: Decoded Message: \"%s\"", s.c_str());
    }
#endif

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
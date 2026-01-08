//
// Created by 79933 on 2025/12/15.
//

#ifndef FLEXRAYTESTDEMO_FLEXRAYLOG_H
#define FLEXRAYTESTDEMO_FLEXRAYLOG_H

#include <iostream>
#include <string>
#include <sstream>


namespace TosunFlexRay{
    template<typename T>
    std::string ToString(const T &value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }


    inline void AppendToStream(std::ostringstream &oss) {
    }


    template<typename First, typename... Rest>
    void AppendToStream(std::ostringstream &oss, const First &first, const Rest &... rest) {
        oss << ToString(first) << ' ';
        AppendToStream(oss, rest...);
    }

    template<typename... Args>
    void Log(bool is_log, const std::string &type, const Args &... args) {
        if (is_log) {
            std::ostringstream oss;
            AppendToStream(oss, args...);
            std::cout << '[' << type << "] " << oss.str() << std::endl;
        }
    }
}

#endif //FLEXRAYTESTDEMO_FLEXRAYLOG_H

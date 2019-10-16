#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <sstream>
class Log
{
public:
    Log();    
    ~Log();
    template<typename T> static void i(T infoStr);
    template<typename T> static void i(T tagStr, T infoStr);
    template<typename T> static void i(T tagStr, T infoStr, T infoStr1);
    template<typename T> static void e(T tagStr);
    template<typename T> static void e(T tagStr, T infoStr);
    template<typename T> static void e(T tagStr, T infoStr, T infoStr1);
};

Log::Log()
{
}

Log::~Log()
{
}

template<typename T> void Log::i(T infoStr) {
    std::cout << "[INFO] "  << infoStr << std::endl;
}

template<typename T> void Log::i(T tagStr, T infoStr) {
    std::cout << "[INFO][" << tagStr<<"] " << infoStr << std::endl;
}

template<typename T> void Log::i(T tagStr, T infoStr, T infoStr1) {
    std::cout << "[INFO][" << tagStr << "] " << infoStr <<" "<< infoStr1<<std::endl;
}

template<typename T> void Log::e(T infoStr) {
    std::cout << "[ERROR] " << infoStr << std::endl;
}

template<typename T> void Log::e(T tagStr, T infoStr) {
    std::cout << "[ERROR][" << tagStr << "] " << infoStr << std::endl;
}

template<typename T> void Log::e(T tagStr, T infoStr, T infoStr1) {
    std::cout << "[INFO][" << tagStr << "] " << infoStr << " " << infoStr1 << std::endl;
}

#endif // !LOG_H


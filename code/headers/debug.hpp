#include <chrono>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#pragma once
//#define DEBUG

#ifdef DEBUG



#define TIMERSTART() auto __clock_start = std::chrono::high_resolution_clock::now();
#define TIMEREND() \
auto __clock_end = std::chrono::high_resolution_clock::now();\
uint64_t __clock_duration = std::chrono::duration_cast<std::chrono::microseconds>(__clock_end - __clock_start).count();\
if (Debug::debug_timer.find(__FUNCTION__) == Debug::debug_timer.end()) {Debug::debug_timer.insert({__FUNCTION__, 0});}\
Debug::debug_timer.at(__FUNCTION__) += __clock_duration;

class Debug {
    public:
        inline static std::map<std::string, uint64_t> debug_timer = {};
        static std::vector<std::string> print_timer(int frameCount) {
            std::vector<std::string> ret;
            for (auto& [key, duration] : debug_timer) {
                ret.push_back(key + ": " + std::to_string(duration/frameCount) + " us per frame");
                duration = 0;
            }
            return ret;
        }
};


#else

#define TIMERSTART()
#define TIMEREND()
class Debug {
    public:
        static std::vector<std::string> print_timer(int frameCount) {
            return {};
        }
};

#endif


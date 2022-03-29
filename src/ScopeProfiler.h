#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include <chrono>

struct ScopeProfiler
{
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    bool done = false;
    
    ScopeProfiler(const std::string& p_name) :
    name(p_name)
    {
        start = std::chrono::high_resolution_clock::now();
    }
    
    std::string end()
    {
        const auto stop = std::chrono::high_resolution_clock::now();
        std::stringstream ss;
        ss << "Scope " << name << ": " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "us";
        done = true;
        return ss.str();
    }
    
    ~ScopeProfiler()
    {
        if(done) return;
        
        std::cout << end() << std::endl;
    }
};

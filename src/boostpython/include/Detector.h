#ifndef SLDETECTOR
#define SLDETECTOR

#include <string>

struct Detector
{
    Detector(std::string name) : name(name), value() {}
    std::string const name;
    float value;
};

#endif
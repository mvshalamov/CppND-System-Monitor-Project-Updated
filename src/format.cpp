#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    int h = seconds / (60 * 60);
    seconds -= h * (60 * 60);

    int m = seconds / 60;
    seconds -= m * 60;
   
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << h << ":" 
    << std::setw(2) << std::setfill('0') << m << ":"
    << std::setw(2) << std::setfill('0') << seconds;
    return ss.str();
}
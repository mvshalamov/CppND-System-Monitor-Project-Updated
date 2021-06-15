#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// /proc/uptime
// #1 uptime of the system (seconds)
// /proc/[PID]/stat
// #14 utime - CPU time spent in user code, measured in clock ticks
// #15 stime - CPU time spent in kernel code, measured in clock ticks
// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
// #22 starttime - Time when the process started, measured in clock ticks
//
// total_time = utime + stime
// total_time = total_time + cutime + cstime
// seconds = uptime - (starttime / Hertz)
// cpu_usage = 100 * ((total_time / Hertz) / seconds)

float Process::CpuUtilization() {
    std::vector<std::string> data = LinuxParser::ParseStatFileName(pid_);
    long total_time = LinuxParser::ActiveJiffies(pid_);

    long seconds = LinuxParser::UpTime() - (std::stoll(data[21]) / sysconf(_SC_CLK_TCK));

    float cpu_usage = 100 * ((float)total_time / sysconf(_SC_CLK_TCK) / seconds);
    
    cpu_usage_ = cpu_usage;

    return cpu_usage_;
}

// DONE: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    return std::to_string(std::stoll(LinuxParser::Ram(pid_)) / 1000); 
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return cpu_usage_ < a.cpu_usage_; 
}

Process::Process(int pid) {pid_ = pid;}
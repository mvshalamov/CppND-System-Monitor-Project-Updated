#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// DONE: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem(); 
}

// get information from /proc/stat on processes
int System::ParseProcInfo(std::string key_name) {
    std::string line, key, value{"0"};
    int return_value{0};
    std::string::size_type sz;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == key_name) {
            return std::stol(value, &sz);
            }
        }
        }
    }
    return return_value;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return ParseProcInfo("procs_running");
 }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
    return ParseProcInfo("processes");
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "iostream"

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float total_memory = GetMemoryInfo("MemTotal");
  float free_memory = GetMemoryInfo("MemFree");

  return (total_memory - free_memory) / total_memory; 
}

float LinuxParser::GetMemoryInfo(std::string key) {
  std::string line, name, data, size_type;
  float mem;

  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
          if (line.rfind(key, 0) == 0) {
              std::istringstream linestream(line);
              linestream >> name >> data >> size_type;
              std::replace(line.begin(), line.end(), ' ', '\0');
              mem = std::stof(data);
              break;
          }
      
      }
  }
  return mem;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string suptime;
  string line;
  std::string::size_type sz; 
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suptime;
  }
  long luptime = std::stol (suptime, &sz);
  return luptime;
}

// ВЩТУ: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long num_jiffies = 0;
  vector<string> cpu_time = LinuxParser::CpuUtilization();
  for (int i = 0; i < 8; i++)
    num_jiffies += std::stol(cpu_time[i]);
  return num_jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::vector<std::string> data = LinuxParser::ParseStatFileName(pid);
  return std::stoll(data[13]) + std::stoll(data[14]) + std::stoll(data[15]) + std::stoll(data[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {

    std::string line, data;
    vector<string> processor_data;

    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            if (line.rfind("cpu", 0) == 0) {
                std::istringstream linestream(line);
                bool i = false;
                while (linestream >> data) {
                  if (i != false) {
                    processor_data.push_back(data);
                  } else {
                    i = true;
                  }
                }
                break;
            }
        
        }
    }
  return processor_data;
}

// get information from /proc/stat on processes
int LinuxParser::ParseProcInfo(std::string key_name) {
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

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return LinuxParser::ParseProcInfo("processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return LinuxParser::ParseProcInfo("procs_running");
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid[[maybe_unused]]) {
  std::string line;
  std::vector<std::string> data;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::string line, name, el, sz;
  std::vector<std::string> data;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.rfind("VmSize", 0) == 0) {
        std::istringstream linestream(line);
        linestream >> name >> el >> sz;
        break;
      }
      
    }
  }
  if (el.empty()) return "0";
  return el; 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line, name, el;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.rfind("Uid:", 0) == 0) {
        std::istringstream linestream(line);
        linestream >> name >> el;
        break;
      }
      
    }
  }
  return el; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user, x, uid, line;
  std::ifstream filestream(kPasswordPath); //parsing "/etc/passwd"
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid){
        if (uid == LinuxParser::Uid(pid)){
          return user;
        }
      }
    }
  }
  return string();
}

std::vector<std::string> LinuxParser::ParseStatFileName(int pid) {
  std::string line, el;
  std::vector<std::string> data;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> el) {
      data.push_back(el);
    }
  }
  return data;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::vector<std::string> data = LinuxParser::ParseStatFileName(pid);
  return LinuxParser::UpTime() - std::stoll(data[21]) / sysconf(_SC_CLK_TCK); 
}

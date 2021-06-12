#include "processor.h"
#include <vector>
#include <string>

#include "linux_parser.h"

using std::vector;
using std::string;

// DONE: Return the aggregate CPU utilization
// user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
/*
    https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald
*/
float Processor::Utilization() {
    vector<string> processor_data_str = LinuxParser::CpuUtilization();
    vector<int> processor_data;
    for (string el : processor_data_str) processor_data.push_back(std::stoi(el));
    
    int prev_idle = prev_processor_data[3] + prev_processor_data[4];
    int idle = processor_data[3] + processor_data[4];

    int prev_non_idle = prev_processor_data[0] + prev_processor_data[1] + \ 
        prev_processor_data[2] + prev_processor_data[5] + prev_processor_data[6] + \
        prev_processor_data[7];

    int non_idle = processor_data[0] + processor_data[1] + \ 
        processor_data[2] + processor_data[5] + processor_data[6] + \
        processor_data[7];
    
    int prev_total = prev_idle + prev_non_idle;
    int total = idle + non_idle;

    int totald = total - prev_total;
    int idlead = idle - prev_idle;

    prev_processor_data = processor_data;
    return (float)(totald - idlead)/totald; 
}
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    std::vector<int> prev_processor_data{0,0,0,0,0,0,0,0,0,0};
};

#endif
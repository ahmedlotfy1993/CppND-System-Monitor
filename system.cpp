#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() 
{ 
 processes_.clear();
 vector<int> pids = LinuxParser::Pids();
 string ram_process ="";
 string cmd_process =""; 
 for (int& pid:pids)
 {
  //Process process(pid); 
  //processes_.push_back(process);
  ram_process = LinuxParser::Ram(pid);
  cmd_process = LinuxParser::Command(pid);
  if((!(ram_process.empty())) && (!(cmd_process.empty())))
  {
   processes_.emplace_back(pid);
  }
 }
 std::sort(processes_.begin(), 
           processes_.end(), 
           std::less<Process>()); 
 return processes_; 
}

std::string System::Kernel() 
{ 
 return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() 
{ 
 return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() 
{ 
 return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() 
{ 
 return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() 
{ 
 return LinuxParser::TotalProcesses(); 
}


long int System::UpTime() 
{ 
 return LinuxParser::UpTime(); 
}
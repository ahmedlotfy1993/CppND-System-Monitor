#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid):pid_(pid)
{
 cpuUtilization_ = CpuUtilization();
}

int Process::Pid() 
{ 
 return pid_; 
}

float Process::CpuUtilization() 
{
 float processor_starttime = (float)LinuxParser::UpTime();
  
 float total_processtime = 
   (float)LinuxParser::ActiveJiffies(pid_);
 total_processtime = total_processtime / sysconf(_SC_CLK_TCK);
  
 float process_starttime = (float)LinuxParser::UpTime(pid_);
 process_starttime = process_starttime / sysconf(_SC_CLK_TCK); 
 
 cpuUtilization_ = 
 total_processtime /(processor_starttime - process_starttime);
 
 return cpuUtilization_;
}

string Process::Command() 
{ 
 return LinuxParser::Command(pid_); 
}

string Process::Ram() 
{ 
 return LinuxParser::Ram(pid_); 
}

string Process::User() 
{ 
 return LinuxParser::User(pid_); 
}

long int Process::UpTime() 
{ 
 return (LinuxParser::UpTime() - 
         LinuxParser::UpTime(pid_)/ sysconf(_SC_CLK_TCK)); 
}

bool Process::operator<(Process const& a) const 
{ 
 return(this->cpuUtilization_ < a.cpuUtilization_);
}
#include "processor.h"
#include "linux_parser.h"
#include <cmath>

float Processor::Utilization() 
{
 float CPU_Percentage = 0.0;
 NonIdle = LinuxParser::ActiveJiffies();
 Idle = LinuxParser::IdleJiffies();
 float Total = (float)(Idle + NonIdle);
 float PrevTotal = (float)(PrevIdle + PrevNonIdle);
 CPU_Percentage = 
 (fabs(Total - PrevTotal) - fabs(Idle - PrevIdle))/
  fabs(Total - PrevTotal);
  PrevIdle = Idle;
  PrevNonIdle = NonIdle;
  return CPU_Percentage;
}
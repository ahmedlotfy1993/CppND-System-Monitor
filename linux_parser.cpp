#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, version,kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >>kernel;
  }
  return kernel;
}


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

float LinuxParser::MemoryUtilization() 
{
  float mem_utilize = 0.0;
  string line;
  string memory_name, memoryvaluestr;
  int memtotal=0;
  int memfree=0;
  std::istringstream linestream;
  std::ifstream stream (kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
   std::getline(stream,line);
   std::replace(line.begin(), line.end(), ':', ' ');
   linestream.str(line);
   linestream >> memory_name >> memoryvaluestr;
   memtotal = std::stoi(memoryvaluestr);
    
   linestream.clear(); 
   std::getline(stream,line);
   std::replace(line.begin(), line.end(), ':', ' ');
   linestream.str(line);
   linestream >> memory_name >> memoryvaluestr;
   memfree = std::stoi(memoryvaluestr); 
  }
  mem_utilize = (float)(memtotal - memfree)
               /(float)(memtotal);
  return mem_utilize; 
}


long LinuxParser::UpTime() 
{
 string line;
 string uptimestr;
 long uptime =0; 
 std::ifstream stream (kProcDirectory + kUptimeFilename);
 if(stream.is_open())
 {
  std::getline(stream,line);
  std::istringstream linestream(line);
  linestream >> uptimestr;
 }
 uptime = std::stoi(uptimestr);
 return uptime;  
}

long LinuxParser::Jiffies() 
{
 long jiffies = 0;
 vector <string>cpuval =LinuxParser::CpuUtilization();
 for (auto i:cpuval)
   jiffies = jiffies + std::stoi(i);
  
 return jiffies; 
}


long LinuxParser::ActiveJiffies(int pid) 
{
 string line, name, val;
 vector<string> pid_str {};
 long processjiffies = 0;
 std::ifstream stream(kProcDirectory + 
                      to_string(pid) + 
                      kStatFilename);
 if (stream.is_open())
 {
  std::getline(stream, line);
  std::istringstream linestream(line);
  for (int i =0 ; i < 17 ; i++)
  {
   linestream >> val ;
   pid_str.push_back(val);
  }
 }
 processjiffies = std::stoi(pid_str[13]) +
                  std::stoi(pid_str[14]) +
                  std::stoi(pid_str[15]) +
                  std::stoi(pid_str[16]);
  
  return processjiffies; 
}


long LinuxParser::ActiveJiffies() 
{
 vector <string>cpuval =LinuxParser::CpuUtilization();
 long userval = std::stoi(cpuval[CPUStates::kUser_]);
 long niceval = std::stoi(cpuval[CPUStates::kNice_]);
 long systemval = std::stoi(cpuval[CPUStates::kSystem_]);
 long irqval = std::stoi(cpuval[CPUStates::kIRQ_]);
 long softirqval = std::stoi(cpuval[CPUStates::kSoftIRQ_]);
 long stealval = std::stoi(cpuval[CPUStates::kSteal_]);
 long activejiffies = userval + niceval + systemval +
                      irqval + softirqval + stealval;
 return activejiffies ; 
}


long LinuxParser::IdleJiffies() 
{
 vector <string>cpuval =LinuxParser::CpuUtilization();
 long idleval = std::stoi(cpuval[CPUStates::kIdle_]);
 long iowaitval = std::stoi(cpuval[CPUStates::kIOwait_]);
 return idleval + iowaitval ; 
}


vector<string> LinuxParser::CpuUtilization() 
{
 vector <string>cpu{};
 string line;
 string word, val; 
 std::ifstream stream (kProcDirectory + kStatFilename);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  { 
   std::istringstream linestream(line);
   linestream >> word;
   if(word == filterCpu)
   {
    while(linestream >> val)
    cpu.push_back(val);
   }
  }
 }
 return cpu; 
}


int LinuxParser::TotalProcesses() 
{
 string line;
 string word,val; 
 int numprocesses =0;  
 std::ifstream stream (kProcDirectory + kStatFilename);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  {
   std::istringstream linestream(line);
   linestream >> word >> val;
   if(word == filterProcesses)
   {
    numprocesses = std::stoi(val);
     break;
   }
  }
 }
  return numprocesses; 
}


int LinuxParser::RunningProcesses() 
{
 string line;
 string word,val; 
 int numprocessesrunning =0;  
 std::ifstream stream (kProcDirectory + kStatFilename);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  {
   std::istringstream linestream(line);
   linestream >> word >> val;
   if(word == filterRunningProcesses)
   {
    numprocessesrunning  = std::stoi(val);
     break;
   }
  }
 }
  return numprocessesrunning;  
}


string LinuxParser::Command(int pid) 
{
 string line=""; 
 std::ifstream stream (kProcDirectory +
                       to_string(pid) +
                       kCmdlineFilename);
 if(stream.is_open())
 {
  std::getline(stream,line);
 }
 return line; 
}


string LinuxParser::Ram(int pid) 
{
 string line="";
 string word="";
 string memorystr="";
 long memoryval = 0; 
  std::ifstream stream (kProcDirectory +
                       to_string(pid) +
                       kStatusFilename);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  {
   std::istringstream linestream(line);
   linestream >> word >> memorystr;
   if(word == filterProcMem)
   {
    memoryval = std::stoi(memorystr);
    break;
   }   
  }
 }
  memoryval = memoryval /(float)1024;
  memorystr = to_string(memoryval);
  return memorystr;
}


string LinuxParser::Uid(int pid) 
{
 string line;
 string word,uid; 
 std::ifstream stream (kProcDirectory +
                       to_string(pid) +
                       kStatusFilename);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  {
   std::replace(line.begin(), line.end(), ':', ' '); 
   std::istringstream linestream(line);
   linestream >> word;
   if(word == filterUID)
   {
    linestream >> uid;
     break;
   }
  }
 }
 return uid; 
}


string LinuxParser::User(int pid) 
{
 string line;
 string username,x,uidpasswd; 
 std::ifstream stream (kPasswordPath);
 string uid =  LinuxParser::Uid(pid);
 if(stream.is_open())
 {
  while(std::getline(stream,line))
  {
   std::replace(line.begin(), line.end(), ':', ' '); 
   std::istringstream linestream(line);
   linestream >> username >> x >>uidpasswd;
   if(uidpasswd == uid)
   {
    break;
   }
  }
 }
 return username;  
}


long LinuxParser::UpTime(int pid) 
{
 string line;
 string val;
 int i= 0;
 long uptime = 0; 
 std::ifstream stream (kProcDirectory +
                       to_string(pid) +
                       kStatFilename);
 if(stream.is_open())
 {
   std::getline(stream,line);
   std::istringstream linestream(line);
   for(i=0; i<22; i++)
    linestream >>val;
 }
 uptime = std::stoi(val); 
 return uptime; 
}
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
//==
#include "linux_parser.h"
//==

using std::string;
using std::to_string;
using std::vector;

//==
Process::Process(int pid) { pid_ = pid; };				// constructor to build the process data
//==

// DONE: Return this process's ID
//==int Process::Pid() { return 0; }
int Process::Pid() {return pid_; }

// DONE: Return this process's CPU utilization
//==float Process::CpuUtilization() { return 0; }
float Process::CpuUtilization() {
  long pid_cpu, sys_up_time, proc_start_time, elapse_time;
//  float cpu_usage = 0;

  pid_cpu = LinuxParser::ActiveJiffies(Pid());      	// get the CPU usage (jiffs) for this process ID
  sys_up_time = LinuxParser::UpTime();              	// get the system up time in seconds
  proc_start_time = LinuxParser::UpTime(Pid());			// get the process up time in seconds
  elapse_time = sys_up_time - proc_start_time; 			// get the elapsed time in seconds since this process started
  if (elapse_time) {									// make sure elapse_time is not zero
//    cpu_usage = float(pid_cpu) / float(elapse_time);	// calculate the % process CPU usage
	cpu_util_ = float(pid_cpu) / float(elapse_time);	// calculate the % process CPU usage
  }
  else { cpu_util_ = 0.0; }
//  return cpu_usage;
  return cpu_util_;
}
//==                                 

// DONE: Return the command that generated this process
//==string Process::Command() { return string(); }
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
//==string Process::Ram() { return string(); }
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
//==string Process::User() { return string(); }
string Process::User() { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
//==long int Process::UpTime() { return 0; }
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
//==bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
bool Process::operator<(Process const& a) const { return cpu_util_ < a.cpu_util_; }
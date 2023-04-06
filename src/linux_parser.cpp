#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
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
  string os, kernel, version;
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
//==float LinuxParser::MemoryUtilization() { return 0.0; }
float LinuxParser::MemoryUtilization() { 
  string line; 									// temp variable to store lines from files
  string label, value;					 		// temp variables to store the data label & value
  float mem_total, mem_free;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {	// parse the label and the value from the line
        if (label == "MemTotal:") {
          mem_total = stof(value);
        }
		if (label == "MemFree:") {
          mem_free = stof(value);
        }
      }
    }
  }
  return (mem_total - mem_free) / mem_total;	// return memory used as a % of total memory
}
//==

// DONE: Read and return the system uptime
//==long LinuxParser::UpTime() { return 0; }
long LinuxParser::UpTime() {
  string line; 									// temp variable to store lines from files
  string value;					 				// temp variable to store the value
  long up_time;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    up_time = stoi(value);
  }
  return up_time;
}
//==

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//==long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }
long LinuxParser::ActiveJiffies(int pid) {
  string line; 									// temp variable to store lines from files
  string value;					 				// temp variable to store the data value 
  vector<string> ticks {};
  long total;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	    std::istringstream linestream(line);
      while (linestream >> value) {
        ticks.emplace_back(value);
      }
    }
  }
  total = stoi(ticks[13]) + stoi(ticks[14]) + stoi(ticks[15]) + stoi(ticks[16]);  // total=utime+stime+cutime+cstime per stack overflow
  return total / sysconf(_SC_CLK_TCK);
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
//==vector<string> LinuxParser::CpuUtilization() { return {}; }
vector<string> LinuxParser::CpuUtilization() {
  string line;           						// temp variable to store lines from files
  string label, value;	      					// temp variables to store the data label & value
  vector<string> jiffs;							// vector to store the 10 timer/jiff values
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);				// get a line from the file
    std::istringstream linestream(line);
    linestream >> label;						// parse the cpu label from the line
	while (linestream >> value) {				// keep parsing values from the line
      jiffs.emplace_back(value);				// add the value to the vector
    }
  }
return jiffs;
}
//==

// DONE: Read and return the total number of processes
//==int LinuxParser::TotalProcesses() { return 0; }
int LinuxParser::TotalProcesses() { 
  string line; 									// temp variable to store lines from files
  string label, value;					 		// temp variables to store the data label & value
  int total_procs; 

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {	// parse the label and the value from the line
        if (label == "processes") {
          total_procs = stoi(value);
        }
      }
    }
  }
  return total_procs;
}
//==

// DONE: Read and return the number of running processes
//==int LinuxParser::RunningProcesses() { return 0; }
int LinuxParser::RunningProcesses() {
  string line; 									// temp variable to store lines from files
  string label, value;					 		// temp variables to store the data label & value
  int running_procs; 

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {	// parse the label and the value from the line
        if (label == "procs_running") {
          running_procs = stoi(value);
        }
      }
    }
  }
  return running_procs;
}
//==

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//==string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Command(int pid) {
  string line; 									// temp variable to store lines from files
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}
//==

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
//==string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Ram(int pid) { 
  string line; 									// temp variable to store lines from files
  string label, value;					 		// temp variables to store the data label & value 
  long ram;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {	// parse the label and the value from the line
        if (label == "VmSize:") {
          ram = stol(value) / 1000;				// convert kilobyte value to megabytes
        }
      }
    }
  }
  return to_string(ram);
}
//==

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//==string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Uid(int pid) {
  string line; 									// temp variable to store lines from files
  string label, value;					 		// temp variables to store the data label & value 
  string user;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {	// parse the label and the value from the line
        if (label == "Uid:") {
          user = value;
        }
      }
    }
  }
  return user;
}
//==

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//==string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::User(int pid) {
  string line; 									// temp variable to store lines from files
  string label, value, id_value;		 		// temp variables to store the data label & value 
  string uid = Uid(pid);						// get the user ID for this process
  string user;
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
      std::replace(line.begin(), line.end(), ':', ' ');
 	  std::istringstream linestream(line);
      while (linestream >> label >> value >> id_value) {	// parse the label and the value from the line
        if (id_value == uid) {								// find the value that matches the User ID
          user = label;										// get the name for the User ID 
        }
      }
    }
  }
  return user;
}
//==

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
//==long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
long LinuxParser::UpTime(int pid) {
  string line; 									// temp variable to store lines from files
  string value;					 				// temp variable to store the data value 
  vector<string> ticks {};

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {	// while the file still has lines to read
 	    std::istringstream linestream(line);
      while (linestream >> value) {
        ticks.push_back(value);
      }
    }
  }
  return stol(ticks[21]) / sysconf(_SC_CLK_TCK);	// return the starttime (in seconds) per stack overflow
}
//==

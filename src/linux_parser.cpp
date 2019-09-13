#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <regex>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

//http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html

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
  filestream.close();
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
  stream.close();
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
        /*
         * Try using emplace_back instead of push_back, it is much more efficient than push_back.
            push_back constructs a temporary object which then will need to get moved into the
            vector v whereas emplace_back just forwards the argument and construct it directly
            in place with no copies or moves needed.
            In short, it is a good habit if you are using emplace_back instead of push_back
         */
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  //cout<<"pids.size() = "<<pids.size()<<"\n";
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value, magnitud;
  float memTotal, memFree;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream>>key>>value>>magnitud) {
        if (key == "MemTotal:") {
          memTotal = stof(value);
        } else if (key == "MemFree:") {
          memFree = stof(value);
          break;
        }
      }
    }
  }
  filestream.close();
  float percent = (memTotal-memFree)/memTotal;
  return percent; 
}  

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  //std::cout<<"Calling UpTime in linux_parser\n";
  string suspendtime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suspendtime >> idletime;
  }
  std::string::size_type sz;
  //std::cout<<"suspendtime: "<<suspendtime<<"\n";
  long lsuspendedtime = stol(suspendtime, &sz);
  //std::cout<<"lsuspendedtime = "<<lsuspendedtime<<"\n";
  stream.close();
  return lsuspendedtime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string line, token;
    vector<string> values;
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                             LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (linestream >> token) {
            values.emplace_back(token);
        }
    }
    long jiffies{0};
    if (values.size() > 21) {
        long user = stol(values[13]);
        long kernel = stol(values[14]);
        long children_user = stol(values[15]);
        long children_kernel = stol(values[16]);
        jiffies = user + kernel + children_user + children_kernel;
    }
    return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    vector<string> time = CpuUtilization();
    return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
            stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
            stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
            stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> time = CpuUtilization();
    return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value = "-1";
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  filestream.close();
  return stoi(value);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value = "-1";
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  filestream.close();
  return stoi(value); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    string line;
    string cmd = " ";
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream>>cmd;
    }
    filestream.close();
    //cout<<"cmd = "<<cmd<<"\n";
    return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// Algunos procesos tienen esto
string LinuxParser::Ram(int pid) {
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    string line, key, value, magnitud;
    if (filestream.is_open()) {
        std::regex reg("(VmSize:)(.*)");
        while (std::getline(filestream, line)) {
            if (std::regex_match(line, reg)) {
                std::istringstream linestream(line);
                linestream>>key>>value>>magnitud;
                break;
            }
        }
    }
    char* endptr = NULL;
    long long int size_kb = std::strtoll(value.c_str(), &endptr, 10);
    size_kb = float(size_kb/1024);
    std::ostringstream out;
    out<<size_kb;
    filestream.close();
    return out.str();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    string line;
    string Uid = "";
    for (int lineNum = 1; std::getline(filestream, line); lineNum++) {
        std::stringstream ss(line);
        string word;
        //std::cout<<"Line # "<<lineNum<<":";
        for (int wordNumb = 1; ss>>word; wordNumb++) {
            if (word == "Uid:") {
                ss>>Uid;
                //cout<<wordNumb<<": "<<word<<" Uid: "<<Uid<<"\n";
                return Uid;
            }
        }
    }
    filestream.close();
    return Uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    std::ifstream filestream(kPasswordPath);
    string line;
    string Uid = LinuxParser::Uid(pid);
    string result = "";
    for (int lineNum = 1; std::getline(filestream, line); lineNum++) {
        std::stringstream ss(line);
        string token;
        char delim = ':';
        vector<string> v;
        while (std::getline(ss, token, delim)) {
            v.emplace_back(token);
            if (token == Uid) {
                result = v[0];
                break;
            }
        }
    }
    filestream.close();
    return result;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    string line, value;
    if (filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream ss(line);
        for (int i = 0; i < 22; i++) {
            ss>>value;
        }
    }
    char* endptr = NULL;
    long int valueTime = std::strtol(value.c_str(), &endptr, 10);
    filestream.close();
    return valueTime;
}
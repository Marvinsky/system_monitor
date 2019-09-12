#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
using std::stoi;
using std::cout;

Process::Process(int _pid) : pid(_pid) {};
// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    string line;
    int UTIME = 13, STIME = 14, CUTIME = 15, CSTIME = 16, STARTTIME = 20;
    //https://stackoverflow.com/questions/1420426/how-to-calculate-the-cpu-usage-of-a-process-by-pid-in-linux-from-c/1424556#1424556
    unordered_map<int, int> data = {
            {UTIME,     0}, //user mode jiffies
            {STIME,     0}, //kernel mode jiffies
            {CUTIME,    0}, //user mode jiffies with child's
            {CSTIME,    0}, //kernel mode jiffies with child's
            {STARTTIME, 0}};

    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream iss(line);
        string val;
        for (int i = 1, count = 0; count < (int)data.size(); ++i) {
            iss>>val;
            if (data.find(i) != data.end()) {
                int val_int = stoi(val);
                data[i] = val_int;
                count++;
            }
        }
    }

    long long uptime = UpTime();
    long int hertz = sysconf(_SC_CLK_TCK);
    float total_time = data[UTIME] + data[STIME] +
            data[CUTIME] + data[CSTIME];
    long int seconds = uptime - (data[STARTTIME]/hertz);
    float result = 100*((total_time/hertz)/seconds);
    return result;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return (this->pid > a.pid);
}

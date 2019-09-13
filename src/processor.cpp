#include "processor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "linux_parser.h"

using std::stoi;
using std::cout;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    unsigned int k = 1;
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    float activeTime, idleTime, totalTime, result;
    string tempVariable;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (filestream>>tempVariable) {
            if (k == 2) {
                user = stoi(tempVariable);
            } else if (k == 3) {
                nice = stoi(tempVariable);
            } else if (k == 4) {
                system = stoi(tempVariable);
            } else if (k == 5) {
                idle = stoi(tempVariable);
            } else if (k == 6) {
                iowait = stoi(tempVariable);
            } else if (k == 7) {
                irq = stoi(tempVariable);
            } else if (k == 8) {
                softirq = stoi(tempVariable);
            } else if (k == 9) {
                steal = stoi(tempVariable);
            } else if (k == 10) {
                guest = stoi(tempVariable);
            } else if (k == 11) {
                guest_nice = stoi(tempVariable);
            }
            k++;
        }
        activeTime = user + nice + system + irq + softirq + steal + guest + guest_nice;
        idleTime = idle + iowait;
        totalTime = activeTime + idleTime;
        result = activeTime/totalTime;
    }
    filestream.close();
    return result;
}
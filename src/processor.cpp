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
    string str_cpu,
    str_user,
    str_nice,
    str_system,
    str_idle,
    str_iowait,
    str_irq,
    str_softirq,
    str_steal,
    str_guest,
    str_guest_nice;
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    float activeTime, idleTime, totalTime, result;
    string line;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream>>str_cpu>>str_user>>str_nice>>str_system>>str_idle>>str_iowait>>str_irq>>str_softirq>>str_steal>>str_guest>>str_guest_nice;

        user = stoi(str_user);
        nice = stoi(str_nice);
        system = stoi(str_system);
        idle = stoi(str_idle);
        iowait = stoi(str_iowait);
        irq = stoi(str_irq);
        softirq = stoi(str_softirq);
        steal = stoi(str_steal);
        guest = stoi(str_guest);
        guest_nice = stoi(str_guest_nice);

        activeTime = user + nice + system + irq + softirq + steal + guest + guest_nice;
        idleTime = idle + iowait;
        totalTime = activeTime + idleTime;
        result = activeTime/totalTime;
    }

    return result;
}
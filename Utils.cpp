#include "Utils.h"
#include <ostream>
Process::Process(int time) : arrivalTime(time), burstTime(rand()%10+1), priority(rand()%3) {}
Process::Process() {}
bool Process::operator < (const Process &p) const {
    if(priority!=p.priority)
        return priority<p.priority;
    if(burstTime!=p.burstTime)
        return burstTime>p.burstTime;
    return arrivalTime>p.arrivalTime;
}
std::ostream& operator << (std::ostream &cout, Process &task) {
    cout << "Process: " << task.arrivalTime << ' ' << task.burstTime << ' ' << task.priority << std::endl;
    return cout;
}

CPU::CPU() {}
CPU::CPU(int sTime, int rTime, int p) : idle(1), startTime(sTime), remainTime(rTime), priority(p) {}

std::ostream& operator << (std::ostream &cout, CPU &CPUState) {
    cout << "CPU: " << CPUState.idle << ' ' << CPUState.startTime << ' ' << CPUState.remainTime << ' ' << CPUState.priority << std::endl;
    return cout;
}


bool jobFinish(CPU &CPUState, int time) {
    if(CPUState.startTime+CPUState.remainTime<=time)
        return FINISH;
    return !FINISH;
}

int createJob(int time, Process &task) {
    int random = rand()%4;
    if(random == 0){  // Jobs are being created with a probability of 1/4
        task = Process(time);
        // cout << "Start " << task << endl;
        return !FAIL;
    }
    return FAIL;
}


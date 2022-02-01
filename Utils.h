#ifndef UTILS_H
#define UTILS_H
#include <ostream>
#include <vector>

const int FAIL = 1;
const int FINISH = 1;

class Process
{
public:
    int arrivalTime;
    int burstTime;
    int priority;

    Process(int time);
    Process();
    bool operator < (const Process &p) const;
};
std::ostream& operator << (std::ostream &cout, Process &task);
class CPU {
public:
    bool idle;
    int startTime;
    int remainTime;
    int priority;

    CPU();
    CPU(int sTime, int rTime, int p);
};
std::ostream& operator << (std::ostream &cout, CPU &CPUState);
int createJob(int time, Process &task);
bool jobFinish(CPU &CPUState, int time);
#endif

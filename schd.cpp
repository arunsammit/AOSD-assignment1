#include <bits/stdc++.h>
using namespace std;

#define FAIL 1
#define FINISH 1

class Process {
public:
    int arrivalTime;
    int busrtTime;
    int priority;

    Process(int time) : arrivalTime(time), busrtTime(rand()%10+1), priority(rand()%3) {}
    Process() {}

    bool operator < (const Process &p) const {
        if(priority!=p.priority)
            return priority<p.priority;
        if(busrtTime!=p.busrtTime)
            return busrtTime>p.busrtTime;
        return arrivalTime>p.arrivalTime;
    }
};

class CPU {
public:
    bool idle;
    int startTime;
    int remainTime;
    int priority;

    CPU() {}
    CPU(int sTime, int rTime, int p) : idle(1), startTime(sTime), remainTime(rTime), priority(p) {}
};

bool jobFinish(CPU &CPUState, int time) {
    if(CPUState.startTime+CPUState.remainTime<=time)
        return FINISH;
    return !FINISH;
}

int createJob(int time, Process &task) {
    int random = rand()%4;
    if(!random) {     // Jobs are being created with 1 in every 4 time step
        task = Process(time);
        return !FAIL;
    }
    return FAIL;
}

void agingProcess(vector<Process> &readyQueue, int time) {
    
}

int highPriorityTask(vector<Process> &readyQueue, CPU &CPUState) {
    return -1;
}

void schedule(Process &task, int time) {

}

int main() {
    const int MAXTIME = 30;
    vector<Process> readyQueue;
    CPU CPUState;
    Process task;
    bool idle = 1;
    for(int time=0; time<MAXTIME; time++) {
        if(jobFinish(CPUState, time)==FINISH)
            CPUState.idle = true;
        if(createJob(time, task))
            readyQueue.push_back(task);
        agingProcess(readyQueue, time);
        int newTask = highPriorityTask(readyQueue, CPUState);
        if(newTask!=-1) {
            schedule(readyQueue[newTask], time);
        }
    }
}
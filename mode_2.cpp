#include <bits/stdc++.h>
using namespace std;

#define FAIL 1
#define FINISH 1
#define NUMPRIORITY 3
#define QUEUESIZE 20

class Process {
public:
    int arrivalTime;
    int burstTime;
    int priority;

    Process(int time) : arrivalTime(time), burstTime(rand()%10+1), priority(rand()%3) {}
    Process() {}

    bool operator < (const Process &p) const {
        if(priority!=p.priority)
            return priority<p.priority;
        if(burstTime!=p.burstTime)
            return burstTime>p.burstTime;
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

ostream& operator << (ostream &cout, CPU &CPUState) {
    cout << "CPU: " << CPUState.idle << ' ' << CPUState.startTime << ' ' << CPUState.remainTime << ' ' << CPUState.priority << endl;
    return cout;
}

ostream& operator << (ostream &cout, Process &task) {
    cout << "Process: " << task.arrivalTime << ' ' << task.burstTime << ' ' << task.priority << endl;
    return cout;
}

bool jobFinish(CPU &CPUState, int time) {
    if(CPUState.startTime+CPUState.remainTime<=time)
        return FINISH;
    return !FINISH;
}

int createJob(int time, Process &task) {
    int random = rand()%4;
    if(!random) {     // Jobs are being created with 1 in every 4 time step
        task = Process(time);
        // cout << "Start " << task << endl;
        return !FAIL;
    }
    return FAIL;
}

int getNextTask(queue<Process> priorityQueue[], Process &task) {
    int zero = 0, one = 0, two = 0;
    if(priorityQueue[0].size())
        zero = 50;
    if(priorityQueue[1].size())
        one = 30;
    if(priorityQueue[2].size())
        two = 20;
    int total = zero+one+two;
    if(total==0)
        return 0;
    int random = rand()%total;
    int index;
    if(random<zero)
        index = 0;
    else if(random<zero+one)
        index = 1;
    else
        index = 2;
    task = priorityQueue[index].front();
    priorityQueue[index].pop();
    return 1;
}

void schedule(int time, Process &task, CPU &CPUState) {
    CPUState.idle = false;
    CPUState.startTime = time;
    CPUState.priority = task.priority;
    CPUState.remainTime = task.burstTime;
}

int main() {
    int jobsCreated = 0, jobsServed = 0, jobsLost = 0, contextSwitch = 0;
    const int MAXTIME = 300;
    queue<Process> priorityQueue[NUMPRIORITY];
    CPU CPUState;
    Process task;
    CPUState.idle = true;
    for(int time=0; time<MAXTIME; time++) {
        if(!CPUState.idle and jobFinish(CPUState, time)==FINISH) {
            CPUState.idle = true;
            jobsServed += 1;
        }
        if(!createJob(time, task)) {
            jobsCreated += 1;
            if(task.priority==0)
                priorityQueue[0].push(task);
            else if(task.priority==1)
                priorityQueue[1].push(task);
            else
                priorityQueue[2].push(task);
        }
        if(!CPUState.idle)
            continue;
        if(!getNextTask(priorityQueue, task))
            continue;
        schedule(time, task, CPUState);
    }
    cout << "Number of jobs created: " << jobsCreated << endl;
    cout << "Number of jobs served: " << jobsServed << endl;
    cout << "Number of jobs Lost: " << jobsLost << endl;
    cout << "Number of jobs in Ready Queue: " << priorityQueue[0].size()+priorityQueue[1].size()+priorityQueue[2].size() << endl;
    cout << "Job executing in CPU: " << !CPUState.idle << endl;
    cout << "Number of Preemption: " << contextSwitch << endl;
    return 0;
}
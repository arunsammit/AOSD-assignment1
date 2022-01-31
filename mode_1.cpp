#include <bits/stdc++.h>
using namespace std;

#define FAIL 1
#define FINISH 1
#define AGING 10
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
    // cout << "Jobs " << random << endl; 
    if(!random) {     // Jobs are being created with 1 in every 4 time step
        task = Process(time);
        // cout << "Start " << task << endl;
        return !FAIL;
    }
    return FAIL;
}

int agingPriority(Process &task, int time) {
    return task.priority+(time-task.arrivalTime)/AGING;
}

int highPriorityTask(vector<Process> &readyQueue, CPU &CPUState, int time) {
    int task = -1;
    for(int idx=0; idx<(int)readyQueue.size(); idx++) {
        int newPriority = agingPriority(readyQueue[idx],time);
        if(CPUState.idle or newPriority>CPUState.priority) {
            if(task==-1 or agingPriority(readyQueue[task], time)<newPriority)
                task = idx;
        }
    }
    return task;
}

void preempt(vector<Process> &readyQueue, CPU &CPUState, int time, int &jobsLost) {
    Process task;
    task.arrivalTime = time;
    task.burstTime = CPUState.remainTime-(time-CPUState.startTime);
    task.priority = CPUState.priority;
    if(readyQueue.size()>=QUEUESIZE+1)
        jobsLost += 1;
    else
        readyQueue.push_back(task);
}

void schedule(vector<Process> &readyQueue, int time, int task, CPU &CPUState) {
    CPUState.idle = false;
    CPUState.startTime = time;
    CPUState.priority = agingPriority(readyQueue[task], time);
    CPUState.remainTime = readyQueue[task].burstTime;
    readyQueue.erase(readyQueue.begin()+task);
}

int main() {
    int jobsCreated = 0, jobsServed = 0, jobsLost = 0, contextSwitch = 0;
    const int MAXTIME = 300;
    vector<Process> readyQueue;
    CPU CPUState;
    Process task;
    CPUState.idle = true;
    for(int time=0; time<MAXTIME; time++) {
        if(!CPUState.idle and jobFinish(CPUState, time)==FINISH) {
            CPUState.idle = true;
            jobsServed += 1;
        }
        if(!createJob(time, task)) {
            if(readyQueue.size()>=QUEUESIZE)
                jobsLost += 1;
            else
                readyQueue.push_back(task);
            jobsCreated += 1;
        }
        int newTask = highPriorityTask(readyQueue, CPUState, time);
        // for(auto &p: readyQueue)
        //     cout << p;
        // cout << "Debug" << endl;
        if(newTask!=-1) {
            if(!CPUState.idle) {
                contextSwitch += 1;
                preempt(readyQueue, CPUState, time, jobsLost);
            }
            schedule(readyQueue, time, newTask, CPUState);
        }
        // cout << readyQueue.size() << endl;
        // cout << CPUState;
        // for(auto &p: readyQueue)
        //     cout << p;
        // cout << "FINISH" << endl;
    }
    cout << "Number of jobs created: " << jobsCreated << endl;
    cout << "Number of jobs served: " << jobsServed << endl;
    cout << "Number of jobs Lost: " << jobsLost << endl;
    cout << "Number of jobs in Ready Queue: " << readyQueue.size() << endl;
    cout << "Job executing in CPU: " << !CPUState.idle << endl;
    cout << "Number of Preemption: " << contextSwitch << endl;
    return 0;
}
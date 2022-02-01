#include "Utils.h"
#include <vector>
#include <iostream>

int queue_size = 20;
int ageing = 10;

using namespace std;

int agingPriority(Process &task, int time) {
    return task.priority+(time-task.arrivalTime)/ageing;
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
    if(readyQueue.size()>=queue_size+1)
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
int main(int argc, char const *argv[]) {
    if (argc !=3){
        cout << "Usage: ./mode_2 <queue_size> <ageing_time>" << endl;
        cout << "Since the command line arguments were not provided, the default values will be used." << endl;
        cout << "The default queue size is 20 and the default ageing time is 10." << endl;
    } else {
        queue_size = atoi(argv[1]);
        ageing = atoi(argv[2]);
    }
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
            if(readyQueue.size()>=queue_size)
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
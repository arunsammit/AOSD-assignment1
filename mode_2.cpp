#include <vector>
#include <iostream>
#include <queue>
#include "Utils.h"
using namespace std;

int queue_size = 20;
int num_queues = 3;

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

int main(int argc, char const *argv[]) {
    if (argc !=2){
        cout << "Usage: ./mode_2 <queue_size>" << endl;
        cout << "Since the command line arguments were not provided, the default value will be used." << endl;
        cout << "The default queue size is 20." << endl;
    } else {
        queue_size = atoi(argv[1]);
    }
    int jobsCreated = 0, jobsServed = 0, jobsLost = 0, contextSwitch = 0;
    const int MAXTIME = 300;
    queue<Process> priorityQueue[num_queues];
    queue<Process> readyQueue;
    CPU CPUState;
    Process task;
    CPUState.idle = true;
    for(int time=0; time<MAXTIME; time++) {
        if(!CPUState.idle and jobFinish(CPUState, time)==FINISH) {
            CPUState.idle = true;
            jobsServed += 1;
        }
        int numJobs = rand()%4;
        while(numJobs--) {
            if(!createJob(time, task)) {
                jobsCreated += 1;
                if(task.priority==0)
                    priorityQueue[0].push(task);
                else if(task.priority==1)
                    priorityQueue[1].push(task);
                else
                    priorityQueue[2].push(task);
            }
        }
        if(!getNextTask(priorityQueue, task))
            continue;
        if(readyQueue.size()>=queue_size) {
            jobsLost += 1;
            continue;
        }
        readyQueue.push(task);
        if(CPUState.idle and readyQueue.size()) {
            schedule(time, readyQueue.front(), CPUState);
            readyQueue.pop();
        }
    }
    cout << "Number of jobs created: " << jobsCreated << endl;
    cout << "Number of jobs served: " << jobsServed << endl;
    cout << "Number of jobs Lost: " << jobsLost << endl;
    cout << "Number of jobs in Ready Queue: " << readyQueue.size() << endl;
    cout << "Job executing in CPU: " << !CPUState.idle << endl;
    cout << "Number of Preemption: " << contextSwitch << endl;
    return 0;
}
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
using namespace std;

class readyQueue {
    
}

void process_creation(int signum) {
    cout << "A Process has been created" << endl;
    alarm(1);
}

int main() {
    signal(SIGALRM, process_creation);
    alarm(1);
    while(1);
}
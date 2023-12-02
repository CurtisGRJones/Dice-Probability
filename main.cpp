#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <thread>
#include <queue>
#include <vector>

// Debug
#include<unistd.h> 

#define EXPECTED_ARG_COUNT 5

// TODO put this in its own files
class Lock {
public:
    static std::queue<int> *queue;
    static int *lock;
    // TODO add setters and getters

    static void waitForLock(int threadId) {
        printf("queue empty?: %d\n", Lock::queue->empty());
        printf("queue length: %lu\n", Lock::queue->size());
        if ( *(Lock::lock) == -1 && Lock::queue->empty()) {
            std::printf("Locking for thread %d due to no thread using lock %d\n", threadId, *(Lock::lock));
            // TODO make a setlock function for this
            *(Lock::lock) = threadId;
            return;
        }

        Lock::queue->push(threadId);

        if (*(Lock::lock) != threadId) {
            std::printf("Thread %d waiting for lock\n", threadId);
        }

        while(*(Lock::lock) != threadId){
            if( *(Lock::lock) == -1 && Lock::queue->front() == threadId ) {
                *(Lock::lock) = Lock::queue->front();
                Lock::queue->pop();
            }
        };
    }

    static void unlock(int threadId) {
        if(*(Lock::lock) != -1 && *(Lock::lock) != threadId) {
            std::printf("Thread %d tried to unlock when thread %d was active\n", threadId, *(Lock::lock));
            return;
        }

        std::printf("Thread %d released lock\n", threadId);

        if ( ! Lock::queue->empty() ) {
            *(Lock::lock) = Lock::queue->front();
            Lock::queue->pop();
        } else {
            std::printf("Empty Queue\n");
            *(Lock::lock) = -1;
        }
    }
};

std::queue<int> *Lock::queue = new std::queue<int>();
int *Lock::lock = new int(-1);

bool intagerValidator(const std::string& str){
    return !str.empty() && std::find_if(
        str.begin(), 
        str.end(), 
        [](unsigned char c) { 
            return !std::isdigit(c); 
        }) == str.end();
}

bool itterateDice(std::vector<int> *dice, const int count, const int maxValue) {
    for( int i = 0; i < count; i++ ) {
        if( dice->at(i) < maxValue ) {
            (*dice)[i]++;
            return true;
        } else {
            (*dice)[i] = 1;
            continue;
        }
    }

    return false;
}

int countDice(std::vector<int> *dice, const int count) {
    int total = 0;
    for ( int i = 0; i < count; i++ ) {
        total += (*dice)[i];
    }
    return total;
}

void threadJob(std::vector<int> dice, const int count, double* results, int threadId) {
    int total = countDice(&dice, count);
    printf("Thread %d result: %d\n", threadId, total);
    Lock::waitForLock(threadId);
    results[total - count]++;
    Lock::unlock(threadId);
}

int main(int argc, char **argv) {
    if (argc < EXPECTED_ARG_COUNT) {
        printf("Expected at least %d arguments but only got %d\n", EXPECTED_ARG_COUNT - 1, argc - 1);
        return -1;
    }

    std::string diceCountStr = argv[1];
    std::string diceMaxValueStr = argv[2];
    std::string comparisonType = argv[3];
    std::string valueStr = argv[4];

    if (!intagerValidator(diceCountStr)) {
        printf("Expected first argument to be an intager\n");
        return -1;
    }

    if (!intagerValidator(diceMaxValueStr)) {
        printf("Expected second argument to be an intager\n");
        return -1;
    }

    if (!intagerValidator(valueStr)) {
        printf("Expected fourth argument to be an intager\n");
        return -1;
    }

    int diceCount = std::stoi( diceCountStr );
    int diceMaxValue = std::stoi( diceMaxValueStr );

    // TODO use nCr calculation
    double grandTotal = pow(diceMaxValue, diceCount);
    printf("total: %.0f\n", grandTotal);

    int min = diceCount;
    int max = diceCount * diceMaxValue;

    // TODO valudate expected is within min & max
    // TODO put limits on number of dice eg. no of dice != 0

    std::vector<int> dice = std::vector<int>(diceCount, 1);
    
    int totalsCountLength = max-min + 1;

    double *totalsCount = new double[totalsCountLength];
    std::fill_n(totalsCount, totalsCountLength, 0);
    // TODO reduce this to only track roles needed for calculation

    const int ThreadCount = grandTotal;

    std::thread *threads = new std::thread[ThreadCount];
    int threadId = 0;
    // TODO have threads configurable and each thread have there own queue
    do {
        printf("Queueing job\n");
        threads[threadId] = std::thread([dice, diceCount, totalsCount, threadId]() {
            threadJob(dice, diceCount, totalsCount, threadId);
        }); 
        threadId++;
        if (threadId >= ThreadCount) {
            break;
        }
    } while (itterateDice(&dice, diceCount, diceMaxValue));

    for (int i = 0; i < ThreadCount; i++) {
        threads[i].join();
    }

    for (int i = 0; i <= max-min; i++) {
        printf("%d: %f\n", i+min, totalsCount[i]);
    }
        // TODO ensure cleanup
}
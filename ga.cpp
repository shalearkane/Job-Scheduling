#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
using namespace std;
#define MAX_TASKS 10
#define MAX_PROCESSORS 3

/*
Note :
Processor index starts from 1
Task index starts from 1
So it is necessary to have an empty 0th index
on all data structures.
*/

typedef struct {
    int task;
    int processor;
} gene;

// chromosome
// genes encode task on processor
typedef struct {
    gene genes[MAX_TASKS + 1];
    int average_cost;
    int makespan;
} chromosome;

// task and the associated cost to process on a
// specific processor
typedef struct {
    int task;
    int cost_on_processor[MAX_PROCESSORS + 1];
} process_cost;

// communication cost
typedef struct {
    int to_node;
    int comm_cost;
} comm_cost_pair;

typedef struct {
    int from_node;
    int comm_delay;
} comm_delay_pair;

typedef struct {
    int from_node;
    int to_node;
} comm_pair;

// scheduling details
typedef struct {
    int task;
    int start_time;
    int end_time;
} scheduled_task_details;

typedef struct {
    set<int> completed_tasks;
    vector<stack<scheduled_task_details>> processor_schedule;
} schedule;

const vector<vector<comm_cost_pair>> dag = {
    {},                                           // 0
    {{2, 18}, {3, 12}, {4, 9}, {5, 11}, {6, 14}}, // 1
    {{8, 19}, {9, 16}},                           // 2
    {{7, 23}},                                    // 3
    {{8, 27}, {9, 23}},                           // 4
    {{9, 13}},                                    // 5
    {{8, 15}},                                    // 6
    {{10, 17}},                                   // 7
    {{10, 11}},                                   // 8
    {{10, 13}},                                   // 9
    {}};                                          // 10

const vector<vector<int>> processing_cost = {
    //  1,  2,  3
    {0, 0, 0, 0},    // 0
    {0, 14, 16, 9},  // 1
    {0, 13, 19, 18}, // 2
    {0, 11, 13, 19}, // 3
    {0, 13, 8, 17},  // 4
    {0, 12, 13, 10}, // 5
    {0, 13, 16, 9},  // 6
    {0, 7, 15, 11},  // 7
    {0, 5, 11, 14},  // 8
    {0, 18, 12, 20}, // 9
    {0, 21, 7, 16},  // 10
};

// functions to write:

// feasibility
// average_cost
// makespan
// two operator
// mutation
// crossover

bool feasibility(const chromosome &c) {
    // checks feasibility by simulating
    // the task on the processors

    /*
    Step 1 :
    Calculate Dependency
    */
    map<int, set<int>> dependency_list;
    map<comm_pair, int> communication_delay;

    for (int i = 1; i <= MAX_TASKS; i++) {
        // for every node that communicates to to_node
        // add a dependency of that to_node
        for (const comm_cost_pair a : dag[i]) {
            comm_pair p = {
                .from_node = i,
                .to_node = a.to_node,
            };
            dependency_list[a.to_node].insert(i);
            communication_delay[p] = a.comm_cost;
        }
    }

    /*
    Step 2 :
    Try scheduling the task
    stop if dependency order is not met
    (pause execution on one processor if dependency is to run on different
    processor) (stop trying if dependency is scheduled on the same processor but
    later)
    */
    for (int i = 1; i <= MAX_TASKS; i++) {
        gene g = c.genes[i];
        // take one gene,
        // schedule it to the processor we want
        // get the processing cost
        // and add it to the time
    }
    return true;
}

int main() {
    srand(time(0));
    chromosome c;
    cout << (rand() % 10) + 1;
    return 0;
}
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
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
    gene gene;
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
        // add a dependency of that to to_node
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
    processor)
    (stop trying if dependency is scheduled on the same processor but
    later)
    */

    schedule test_schedule;
    vector<queue<gene>> stalled_tasks_on_processor(4);

    // (task, scheduled_task_details)
    map<int, scheduled_task_details> completed_task_details;

    for (int i = 1; i <= MAX_TASKS; i++) {
        gene g = c.genes[i];
        /*
        take one gene,
        schedule it to the processor it requires
        then get the processing cost
        and add it to the time
        */

        /*
        A processor is stalled when the oldest task in
        stalled_tasked_on_processor can't be executed
        */

        // try finishing the stalled tasks

        // step 1: check dependency
        set<int> dependencies = dependency_list[g.task];
        bool is_all_dependencies_requirements_met = true;
        int when_task_can_start = -1;

        for (int d : dependencies) {
            if (test_schedule.completed_tasks.count(d)) {
                // task is completed

                // get communication delay
                int delay = 0;
                if (completed_task_details[d].gene.processor != g.processor) {
                    comm_pair c_p_temp = {d, g.task};
                    int delay = communication_delay[c_p_temp];
                }
                when_task_can_start =
                    max(completed_task_details[d].end_time + delay,
                        when_task_can_start);
                continue;
            } else {
                is_all_dependencies_requirements_met = false;
                int processor_to_run_dependency_on = -1;

                // get dependency gene
                for (gene d_gene : c.genes) {
                    if (d_gene.task == d) {
                        processor_to_run_dependency_on = d_gene.processor;

                        // check is that task is on the same processor

                        if (d_gene.processor == g.processor) {
                            // not possible to schedule
                            return false;
                        } else {
                            // submit the dependency task on processor queue
                            stalled_tasks_on_processor
                                [processor_to_run_dependency_on]
                                    .push(d_gene);

                            // submit the stalled task to the processor queue
                            stalled_tasks_on_processor[g.processor].push(g);
                        }
                    }
                }
            }
        }

        if (is_all_dependencies_requirements_met) {
            scheduled_task_details stdop_temp;
            stdop_temp.gene = g;
            stdop_temp.start_time =
                max(when_task_can_start,
                    test_schedule.processor_schedule[g.processor]
                        .top()
                        .end_time) +
                1;
            stdop_temp.end_time =
                stdop_temp.start_time + processing_cost[g.task][g.processor];
            test_schedule.processor_schedule[g.processor].push(stdop_temp);
        }
    }
    return true;
}

int main() {
    srand(time(0));
    chromosome c;
    cout << (rand() % 10) + 1;
    return 0;
}
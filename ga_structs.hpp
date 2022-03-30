#pragma once
#include "ga_constants.hpp"
#include <set>
#include <stack>
#include <vector>

typedef struct gene_s {
    int task;
    int processor;
} gene;

// scheduling details
typedef struct scheduled_task_details_s {
    gene g;
    int start_time;
    int end_time;
} scheduled_task_details;

typedef struct schedule_s {
    std::set<int> completed_tasks;
    std::vector<std::vector<scheduled_task_details>> processor_schedule;
} schedule;
// chromosome
// genes encode task on processor
typedef struct chromosome_s {
    gene genes[MAX_TASKS + 1];
    float average_cost;
    int makespan;
    float fitness_value;
    schedule sched;
} chromosome;

// task and the associated cost to process on a
// specific processor
typedef struct process_cost_s {
    int task;
    int cost_on_processor[MAX_PROCESSORS + 1];
} process_cost;

// communication cost
typedef struct comm_cost_pair_s {
    int to_node;
    int comm_cost;
} comm_cost_pair;

typedef struct comm_delay_pair_s {
    int from_node;
    int comm_delay;
} comm_delay_pair;

typedef struct comm_pair_s {
    int from_node;
    int to_node;
} comm_pair;

typedef struct feasibility_details_s {
    bool is_feasible;
    schedule sched;
} feasibility_details;
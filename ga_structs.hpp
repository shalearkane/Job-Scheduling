#include "ga_constants.hpp"
#include <set>
#include <stack>
#include <vector>

typedef struct gene {
    int task;
    int processor;
} gene;

// chromosome
// genes encode task on processor
typedef struct chromosome {
    gene genes[MAX_TASKS + 1];
    float average_cost;
    int makespan;
} chromosome;

// task and the associated cost to process on a
// specific processor
typedef struct process_cost {
    int task;
    int cost_on_processor[MAX_PROCESSORS + 1];
} process_cost;

// communication cost
typedef struct comm_cost_pair {
    int to_node;
    int comm_cost;
} comm_cost_pair;

typedef struct comm_delay_pair {
    int from_node;
    int comm_delay;
} comm_delay_pair;

typedef struct comm_pair {
    int from_node;
    int to_node;
} comm_pair;

// scheduling details
typedef struct scheduled_task_details {
    gene gene;
    int start_time;
    int end_time;
} scheduled_task_details;

typedef struct schedule {
    std::set<int> completed_tasks;
    std::vector<std::stack<scheduled_task_details>> processor_schedule;
} schedule;

typedef struct feasibility_details {
    bool is_feasible;
    schedule schedule;
} feasibility_details;
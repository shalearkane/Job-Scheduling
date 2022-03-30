#include "ga_print.hpp"
#include "ga_constants.hpp"
#include "ga_structs.hpp"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void print_schedule(const schedule s) {
    for (int i = 1; i < s.processor_schedule.size(); i++) {
        cerr << "For processor " << i << " : \n";
        for (int j = 0; j < s.processor_schedule[i].size(); j++)
            cerr << s.processor_schedule[i][j].g.task << "\t"
                 << s.processor_schedule[i][j].end_time << '\n';
    }
}

void print_queue_of_tasks_on_processor(
    const vector<queue<gene>> tasks_on_processor) {
    for (int i = 1; i <= MAX_PROCESSORS; i++) {
        queue<gene> q = tasks_on_processor[i];
        cerr << "Queue " << i << " :\n";
        while (!q.empty()) {
            cerr << q.front().task << ", ";
            q.pop();
        }
        cerr << "\n\n";
    }
}

void print_chromosome(const chromosome c) {
    cerr << "Chromosome : \n";
    for (int i = 1; i <= MAX_TASKS; i++) {
        cerr << c.genes[i].task << "\t" << c.genes[i].processor << "\n";
    }
}

void print_scheduled_task_details(const scheduled_task_details sched_td) {
    cerr << "Task " << sched_td.g.task << " : " << sched_td.start_time << " - "
         << sched_td.end_time << '\n';
}
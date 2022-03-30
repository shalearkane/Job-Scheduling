#include "ga_constants.hpp"
#include "ga_inputs.hpp"
#include "ga_print.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
using namespace std;

vector<set<int>> dependency(dag.size());
vector<float> mean_wt(dag.size(), 0);
vector<float> upward_rank(dag.size(), 0);

chromosome heft_chromosome;

// {task, upward_rank}
vector<pair<int, float>> upward_rank_sorted(dag.size(), {0, 0});

int get_comm_cost(int parent, int child) {
    for (comm_cost_pair parent_node : dag[parent]) {
        if (parent_node.to_node == child)
            return parent_node.comm_cost;
    }
    return -1;
}

// variable to store tasks assigned to processor
// p1 : 1, 2, 5
// p2 : 3
// p3 : 4
vector<set<int>> tasks_to_processor(processing_cost[0].size());

// variable to store final scheduling
// p1 : {task1, finish_time1}, {task2, finish_time2}
// p2 : {task1, finish_time1}, {task2, finish_time2}
// p3 : {task1, finish_time1}, {task2, finish_time2}
// vector<vector<pair<int, int>>> schedule(processing_cost[0].size());

int get_end_time(int task, int processor, schedule s) {
    for (scheduled_task_details st_details : s.processor_schedule[processor]) {
        if (st_details.g.task == task)
            return st_details.end_time;
    }
    return -1;
}

void set_dependency() {
    for (int i = 1; i < dag.size(); i++) {
        for (comm_cost_pair pair : dag[i])
            dependency[pair.to_node].insert(i);
    }
}

void set_mean_wt() {
    for (int i = 1; i < processing_cost.size(); i++) {
        for (auto j : processing_cost[i])
            mean_wt[i] += j;
        mean_wt[i] = mean_wt[i] / (float)processing_cost[i].size();
    }
}

float calculate_upward_rank(int i) {
    if (upward_rank[i] > 0) {
        return upward_rank[i];
    }

    const vector<comm_cost_pair> exiting_edges = dag[i];
    float max_cost = 0;

    for (comm_cost_pair a : exiting_edges) {
        max_cost =
            max(calculate_upward_rank(a.to_node) + a.comm_cost, max_cost);
    }

    upward_rank[i] = max_cost + mean_wt[i];
    return upward_rank[i];
}

void set_upward_rank() {
    for (int i = 1; i < dag.size(); i++) {
        upward_rank_sorted[i].first = i;
        upward_rank_sorted[i].second = calculate_upward_rank(i);
    }
}

void calculate_schedule(schedule &schedule) {
    int heft_gene_counter = 0;

    for (pair<int, float> task : upward_rank_sorted) {
        // minimise the time by testing scheduling on
        // different processors

        // assumptions :
        // dependencies have already completed tasks

        // estimated finish time on different processors
        // {eft,processor}
        vector<pair<int, int>> eft;

        // iterating through each processor and storing on eft;
        for (int i = 1; i < processing_cost[task.first].size(); i++) {
            // running on processor i

            if (DEB) {
                cout << "Scheduling : " << task.first << " on " << i << '\n';
            }
            // step 1 : calculate communication costs from each dependency
            vector<int> comm_eft(processing_cost[0].size(), 0);
            for (int dep : dependency[task.first]) {
                // check if dep already scheduled in the same processor
                if (tasks_to_processor[i].count(dep)) {
                    continue;
                } else {
                    for (int i = 1; i < tasks_to_processor.size(); i++) {
                        // add communication costs of each processor separately
                        if (tasks_to_processor[i].count(dep)) {
                            // if a dep has run on processor i
                            const int task_comm_end =
                                get_end_time(dep, i, schedule) +
                                get_comm_cost(dep, task.first);
                            comm_eft[i] = max(task_comm_end, comm_eft[i]);
                            break;
                        }
                    }
                }
            }

            int max_comm_cost = *max_element(comm_eft.begin(), comm_eft.end());

            if (DEB) {
                cout << "comm_cost = " << max_comm_cost << '\n';
            }
            // step 2 : get cost of running that job on that processor
            int running_time = processing_cost[task.first][i];

            if (DEB) {
                cout << "running_time = " << running_time << '\n';
            }
            // step 3 : get max(eft_last_running_process_on_that_processor,
            // end_time_of_dependency)
            int start_time = 0;

            // end_time_of_every_dependency
            for (int j = 1; j < schedule.processor_schedule.size(); j++) {
                // if (DEB) {
                //     cout << "In here";
                // }
                cout << "in here 1";
                for (scheduled_task_details process :
                     schedule.processor_schedule[j]) {
                    cout << "in here 2";
                    if (dependency[task.first].count(process.g.task)) {
                        start_time = max(process.end_time, start_time);
                    }
                }
            }
            start_time =
                max(schedule.processor_schedule[i].back().end_time, start_time);

            if (DEB) {
                cout << "start_time = " << start_time << '\n';
            }

            int end_time = max(start_time, max_comm_cost) + running_time;

            eft.push_back({end_time, i});
        }

        // {eft,processor}
        pair<int, int> min_eft = *min_element(eft.begin(), eft.end());

        assert(min_eft.second < processing_cost[0].size());
        scheduled_task_details st_details;
        st_details.g.task = task.first;
        st_details.g.processor = min_eft.second;
        st_details.end_time = min_eft.first;
        schedule.processor_schedule[min_eft.second].push_back(st_details);
        heft_chromosome.genes[heft_gene_counter].task = task.first;
        heft_chromosome.genes[heft_gene_counter].processor = min_eft.second;
        tasks_to_processor[min_eft.second].insert(task.first);
        // print_schedule();
    }
}

int main() {
    schedule s;
    s.processor_schedule.push_back({});
    s.processor_schedule.push_back({});
    s.processor_schedule.push_back({});
    s.processor_schedule.push_back({});
    assert(dag.size() == processing_cost.size());
    set_dependency();
    set_mean_wt();
    set_upward_rank();
    sort(upward_rank_sorted.begin(), upward_rank_sorted.end(),
         [](auto &left, auto &right) { return left.second > right.second; });
    upward_rank_sorted.pop_back(); // remove the 0th element
    for (int i = 1; i < s.processor_schedule.size(); i++) {
        scheduled_task_details temp;
        temp.g.processor = 0;
        temp.g.task = 0;
        temp.start_time = temp.end_time = 0;
        s.processor_schedule[i].push_back(temp);
    }
    calculate_schedule(s);
    print_schedule(s);

    return 0;
}
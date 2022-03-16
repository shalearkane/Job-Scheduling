#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
using namespace std;

#define DEB 1

const vector<vector<pair<int, int>>> dag = {
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

vector<set<int>> dependency(dag.size());
vector<float> mean_wt(dag.size(), 0);
vector<float> upward_rank(dag.size(), 0);

// {task, upward_rank}
vector<pair<int, float>> upward_rank_sorted(dag.size(), {0, 0});

int get_comm_cost(int parent, int child) {
    for (pair<int, int> a : dag[parent]) {
        if (a.first == child)
            return a.second;
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
vector<vector<pair<int, int>>> schedule(processing_cost[0].size());

void print_schedule() {
    for (int i = 1; i < schedule.size(); i++) {
        cout << "For processor " << i << " : \n";
        for (int j = 0; j < schedule[i].size(); j++)
            cout << schedule[i][j].first << "\t" << schedule[i][j].second
                 << '\n';
    }
}

void set_dependency() {
    for (int i = 1; i < dag.size(); i++) {
        for (auto pair : dag[i])
            dependency[pair.first].insert(i);
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

    const vector<pair<int, int>> exiting_edges = dag[i];
    float max_cost = 0;

    for (pair<int, int> a : exiting_edges) {
        max_cost = max(calculate_upward_rank(a.first) + a.second, max_cost);
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

void calculate_schedule() {
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
            vector<int> comm_cost(processing_cost[0].size(), 0);
            for (int dep : dependency[task.first]) {
                // check if dep already scheduled in the same processor
                if (tasks_to_processor[i].count(dep)) {
                    continue;
                } else {
                    for (int i = 1; i < tasks_to_processor.size(); i++) {
                        // add communication costs of each processor separately
                        if (tasks_to_processor[i].count(dep)) {
                            // if a dep has run on processor i
                            comm_cost[i] += get_comm_cost(dep, task.first);
                            break;
                        }
                    }
                }
            }

            int max_comm_cost =
                *max_element(comm_cost.begin(), comm_cost.end());

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
            for (int j = 1; j < schedule.size(); j++) {
                // if (DEB) {
                //     cout << "In here";
                // }
                for (pair<int, int> process : schedule[j]) {
                    if (dependency[task.first].count(process.first)) {
                        start_time = max(process.second, start_time);
                    }
                }
            }
            start_time = max(schedule[i].back().second, start_time);

            if (DEB) {
                cout << "start_time = " << start_time << '\n';
            }

            int end_time = start_time + max_comm_cost + running_time;

            eft.push_back({end_time, i});
        }

        // {eft,processor}
        pair<int, int> min_eft = *min_element(eft.begin(), eft.end());

        assert(min_eft.second < processing_cost[0].size());
        schedule[min_eft.second].push_back({task.first, min_eft.first});
        tasks_to_processor[min_eft.second].insert(task.first);
        print_schedule();
    }
}

int main() {
    assert(dag.size() == processing_cost.size());
    set_dependency();
    set_mean_wt();
    set_upward_rank();
    sort(upward_rank_sorted.begin(), upward_rank_sorted.end(),
         [](auto &left, auto &right) { return left.second > right.second; });
    upward_rank_sorted.pop_back(); // remove the 0th element
    for (int i = 1; i < schedule.size(); i++) {
        schedule[i].push_back({0, 0});
    }
    calculate_schedule();

    return 0;
}
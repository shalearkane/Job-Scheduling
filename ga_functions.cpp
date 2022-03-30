#include "ga_functions.hpp"
#include "ga_constants.hpp"
#include "ga_inputs.hpp"
#include "ga_structs.hpp"
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
using namespace std;

/*
Note :
Processor index starts from 1
Task index starts from 1
So it is necessary to have an empty 0th index
on all data structures.
*/

// functions to write:

// feasibility_details
// average_cost
// makespan
// mutation
// crossover

feasibility_details feasibility(const chromosome c) {
    cerr << "In here";
    feasibility_details fd;
    // checks feasibility by simulating
    // the task on the processors

    /*
    Step 1 :
    Calculate Dependency
    */
    std::map<int, std::set<int>> dependency_list;
    // map<from_node, map<to_node, delay>>
    std::map<int, std::map<int, int>> communication_delay;

    for (int i = 1; i <= MAX_TASKS; i++) {
        // for every node that communicates to to_node
        // add a dependency of that to to_node
        for (const comm_cost_pair a : dag[i]) {
            comm_pair p = {
                .from_node = i,
                .to_node = a.to_node,
            };
            dependency_list[a.to_node].insert(i);
            communication_delay[p.from_node][p.to_node] = a.comm_cost;
        }
    }

    /*
    Step 2 :
    prepare n queue for jobs to run on n processors
    Then pop one job from top of the queue if its dependencies are met
    */

    schedule test_schedule;
    std::vector<std::queue<gene>> tasks_on_processor(4);

    // (task, scheduled_task_details)
    std::map<int, scheduled_task_details> completed_task_details;
    for (int i = 1; i <= MAX_TASKS; i++) {

        gene g = c.genes[i];

        // queue the genes on their respective processors
        tasks_on_processor[g.processor].push(g);
    }

    /*
    All of the processors are stalled if none of
    the top jobs can be executed
    */
    bool did_anything_run = true;

    while (did_anything_run) {
        did_anything_run = false;

        // q_top -> queue of tasks on processos
        for (std::queue<gene> q : tasks_on_processor) {
            // try to run the top of the queue
            gene g = q.front();

            std::set<int> dependencies = dependency_list[g.task];
            bool is_all_dependencies_requirements_met = true;
            int when_task_can_start = -1;

            for (int d : dependencies) {
                if (test_schedule.completed_tasks.count(d)) {
                    // task is completed
                    // get communication delay
                    int delay = 0;
                    if (completed_task_details[d].g.processor != g.processor) {
                        comm_pair c_p_temp = {d, g.task};
                        int delay = communication_delay[c_p_temp.from_node]
                                                       [c_p_temp.to_node];
                    }
                    when_task_can_start =
                        std::max(completed_task_details[d].end_time + delay,
                                 when_task_can_start);
                    continue;
                } else {
                    // dependency not completed
                    // so wait
                    is_all_dependencies_requirements_met = false;
                    break;
                }
            }

            // schedule the task
            if (is_all_dependencies_requirements_met) {
                // remove the task from top of the queue
                q.pop();
                did_anything_run = true;

                scheduled_task_details st_details;
                st_details.g = g;
                st_details.start_time =
                    std::max(when_task_can_start,
                             test_schedule.processor_schedule[g.processor]
                                 .back()
                                 .end_time) +
                    1;
                st_details.end_time = st_details.start_time +
                                      processing_cost[g.task][g.processor];
                test_schedule.processor_schedule[g.processor].push_back(
                    st_details);
                test_schedule.completed_tasks.insert(g.task);
            }
        }
    }

    int is_feasible = true;
    for (std::queue<gene> q : tasks_on_processor) {
        if (q.size() > 0) {
            is_feasible = false;
            break;
        }
    }
    fd.is_feasible = is_feasible;
    fd.sched = test_schedule;
    return fd;
}

int average_cost(const chromosome chromosomes) {
    int sum = 0;
    int t, p, c;
    for (gene i : chromosomes.genes) {

        t = i.task;
        p = i.processor;
        sum += processing_cost[t][p];
    }

    return sum / (processing_cost.size() - 1);
}

chromosome crossover(const chromosome A, const chromosome B) {
    chromosome C;
    std::set<int> tasks;
    int r = rand() % sizeof(A.genes);
    int counter = 1;
    for (int i = 1; i < r; i++) {
        if (tasks.find(A.genes[i].task) == tasks.end()) {
            C.genes[counter] = A.genes[i];
            counter++;
            tasks.insert(A.genes[i].task);
        }
    }
    for (int i = r; i <= MAX_TASKS; i++) {
        if (tasks.find(B.genes[i].task) == tasks.end()) {
            C.genes[counter] = B.genes[i];
            counter++;
            tasks.insert(B.genes[i].task);
        }
    }
    for (int i = 1; i < r; i++) {
        if (tasks.find(B.genes[i].task) == tasks.end()) {
            // if we don't find the gene
            // then we insert the gene in the same position
            // because if we insert the gene in the last position,
            // because of breaking dependency, it might make the offspring
            // unfit.
            for (int j = MAX_TASKS - 1; j >= i; j--) {
                C.genes[j + 1] = C.genes[j];
            }
            C.genes[i] = B.genes[i];
            tasks.insert(B.genes[i].task);
        }
    }
    return C;
}

chromosome mutation(chromosome off_spring, const float mutation_rate) {
    if ((float)rand() / (float)RAND_MAX <= mutation_rate) {
        // random indexes
        int a = (rand() % MAX_TASKS) + 1;
        int b = (rand() % MAX_TASKS) + 1;
        if (off_spring.genes[a].processor != off_spring.genes[b].processor ||
            off_spring.genes[a].task != off_spring.genes[b].task) {
            std::swap(off_spring.genes[a].processor,
                      off_spring.genes[b].processor);
            std::swap(off_spring.genes[a].task, off_spring.genes[b].task);
        }
    }
    return off_spring;
}

int makespan(const schedule s) {
    int time = 0;
    for (std::vector<scheduled_task_details> st : s.processor_schedule) {
        int end_time = st.back().end_time;
        time = std::max(end_time, time);
    }
    return time;
}

float fitness(const float average_cost, const int make_span) {
    return (1.0 / 1.0 + (make_span * average_cost));
}

void print_chromosome(const chromosome c) {
    std::cout << "Chromosome : \n";
    for (int i = 1; i <= MAX_TASKS; i++) {
        std::cout << c.genes[i].task << "\t" << c.genes[i].processor << "\n";
    }
}
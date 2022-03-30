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
    const std::vector<std::queue<gene>> tasks_on_processor) {
    for (int i = 0; i <= MAX_PROCESSORS; i++) {
        std::queue<gene> q = tasks_on_processor[i];
        while (!q.empty()) {
            std::cerr << q.front().task << " : " << q.front().processor << "\n";
            q.pop();
        }
        std::cerr << '\n';
    }
}

void print_chromosome(const chromosome c) {
    std::cout << "Chromosome : \n";
    for (int i = 1; i <= MAX_TASKS; i++) {
        std::cout << c.genes[i].task << "\t" << c.genes[i].processor << "\n";
    }
}
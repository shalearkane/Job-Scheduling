#include "ga_constants.hpp"
#include "ga_functions.hpp"
#include "ga_heft.hpp"
#include "ga_inputs.hpp"
#include "ga_print.hpp"
#include "ga_structs.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
using namespace std;

vector<chromosome> population_array;
float average_fitness_val;

void population(const chromosome heft) {
    int i = 0;
    chromosome temp;
    temp = mutation(heft, 0.1);
    while (i < MAX_POPULATION - 1) {
        if (feasibility(temp).is_feasible) {
            population_array.push_back(temp);
            i++;
        };
    }
    population_array.push_back(heft);
}

bool cmp_fitness_val(const chromosome &c1, const chromosome &c2) {
    return (c1.fitness_value > c2.fitness_value);
}
void generation() {
    vector<chromosome>::iterator itr;

    for (itr = population_array.begin(); itr != population_array.end(); itr++) {
        itr->average_cost = average_cost(*itr);
    }

    for (itr = population_array.begin(); itr != population_array.end(); itr++) {
        itr->makespan = makespan(itr->sched);
    }

    float sum_fitness;
    for (itr = population_array.begin(); itr != population_array.end(); itr++) {
        itr->fitness_value = fitness(itr->average_cost, itr->makespan);
        sum_fitness += itr->fitness_value;
    }
    average_fitness_val = sum_fitness / (float)population_array.size();

    sort(population_array.begin(), population_array.end(), cmp_fitness_val);
    itr = population_array.begin();

    for (int i = 0; i < 14; i += 2) {
        chromosome temp = mutation(
            crossover(population_array[i], population_array[i + 1]), 0.1);
        if (feasibility(temp).is_feasible) {
            population_array.push_back(temp);
        };
    }
    sort(population_array.begin(), population_array.end(), cmp_fitness_val);
    population_array.resize(MAX_POPULATION);
}

int main() {
    srand(time_t(0));
    chromosome heft = get_heft_chromosome();
    print_genes(heft.genes);
    feasibility_details fd_heft = feasibility(heft);
    int mk_heft = makespan(fd_heft.sched);
    cout << "makespan heft : " << mk_heft << '\n';

    population(heft);
    population_array.push_back(heft);
    for (int i = 0; i < 2000; i++) {
        generation();
    }

    print_genes(population_array.front().genes);
    feasibility_details fd_final = feasibility(population_array.front());
    int mk_final = makespan(fd_final.sched);
    cout << "makespan final : " << mk_final << '\n';

    for (int i = 0; i < population_array.size(); i++) {
        print_genes(population_array[i].genes);
    }

    return 0;
}
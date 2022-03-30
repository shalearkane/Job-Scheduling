#include "ga_constants.hpp"
#include "ga_functions.hpp"
#include "ga_heft.hpp"
#include "ga_inputs.hpp"
#include "ga_print.hpp"
#include "ga_structs.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;

vector<chromosome> population_array;
float average_fitness_val;

void population(chromosome heft) {
    int i = 0;
    chromosome temp;
    temp = mutation(heft, 0.1);
    while (i < MAX_POPULATION - 1) {
        if (feasibility(temp).is_feasible) {
            population_array.push_back(temp);
            i++;
        };
    }
}

bool cmp_fitness_val(chromosome c1, chromosome c2) {
    return (c1.fitness_value < c2.fitness_value);
}
void generation() {
    vector<chromosome>::iterator itr;
    float sum_fitness;
    for (itr = population_array.begin(); itr != population_array.end(); itr++) {
        itr->fitness_value = fitness(itr->average_cost, itr->makespan);
        sum_fitness += itr->fitness_value;
    }
    average_fitness_val = sum_fitness / (float)population_array.size();

    sort(population_array.begin(), population_array.end(), cmp_fitness_val);
}

int main() {
    srand(time(0));
    feasibility_details fd;
    chromosome c = get_heft_chromosome();
    population(c);
    fd = feasibility(c);
    cerr << "\nFeasibility schedule :\n";
    print_schedule(fd.sched);
    cerr << "\nIs feasible : " << fd.is_feasible;
    print_chromosome(c);
    cout << (rand() % 10) + 1;
    chromosome population[20];
    fitness(12, 23);
}